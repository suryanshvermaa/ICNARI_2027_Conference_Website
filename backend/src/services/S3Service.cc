#include "../config/AwsConfig.h"
#include "S3Service.h"

#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <stdexcept>

static std::string requireBucket()
{
    const auto storage = AwsConfig::load();
    if (storage.bucket.empty())
    {
        throw std::runtime_error(
            "S3_BUCKET is not set (or .env was not loaded before S3Service was used)"
        );
    }
    return storage.bucket;
}

/**
 * @details Generates a pre-signed URL for the specified S3 object key. The URL is valid for 1 hour (3600 seconds).
 */
std::string getSignedUrl(const std::string& key){
    const auto bucket = requireBucket();
    Aws::S3::S3Client client(
        AwsConfig::credentials(),
        AwsConfig::clientConfig(),
        Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
        false
    );
    return client.GeneratePresignedUrl(bucket, key, Aws::Http::HttpMethod::HTTP_GET, 3600);
}

/**
 * @details Generates a pre-signed URL for uploading an object to the specified S3 bucket with the given key. The URL is valid for 1 hour (3600 seconds).
 */
std::string putObject(const std::string& key, const drogon::HttpFile& file){
    const auto bucket = requireBucket();
    Aws::S3::S3Client client(
        AwsConfig::credentials(),
        AwsConfig::clientConfig(),
        Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
        false
    );
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucket);
    request.SetKey(key);

    auto body = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
    body->write(file.fileData(), static_cast<std::streamsize>(file.fileLength()));
    request.SetContentLength(static_cast<long long>(file.fileLength()));
    request.SetBody(body);

    auto outcome = client.PutObject(request);
    if (!outcome.IsSuccess()) {
        throw std::runtime_error("Failed to upload object: " + outcome.GetError().GetMessage());
    }
    return key;
}

/**
 * @details Deletes the specified object from the S3 bucket.
 */
bool deleteObject(const std::string& key){
    const auto bucket = requireBucket();
    Aws::S3::Model::DeleteObjectRequest request;
    Aws::S3::S3Client client(
        AwsConfig::credentials(),
        AwsConfig::clientConfig(),
        Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
        false
    );
    request.SetBucket(bucket);
    request.SetKey(key);

    auto outcome = client.DeleteObject(request);
    if (!outcome.IsSuccess()) {
        throw std::runtime_error("Failed to delete object: " + outcome.GetError().GetMessage());
    }

    return true;
}

std::string urlToKey(const std::string& url) {
    const std::string bucket = requireBucket();

    // remove query params
    std::string clean = url.substr(0, url.find('?'));

    // remove protocol
    size_t proto = clean.find("://");
    if (proto == std::string::npos)
        throw std::invalid_argument("Invalid URL");

    std::string rest = clean.substr(proto + 3);

    // split host and path
    size_t slash = rest.find('/');
    if (slash == std::string::npos)
        throw std::invalid_argument("Invalid URL path");

    std::string host = rest.substr(0, slash);
    std::string path = rest.substr(slash + 1); // without leading /

    // CASE 1: virtual hosted style
    // bucket.s3.amazonaws.com/key
    if (host.rfind(bucket + ".", 0) == 0) {
        return path;
    }

    // CASE 2: path style
    // s3.amazonaws.com/bucket/key
    if (path.rfind(bucket + "/", 0) == 0) {
        return path.substr(bucket.size() + 1);
    }

    // CASE 3: custom domain or CDN
    // cdn.example.com/key
    return path;
}