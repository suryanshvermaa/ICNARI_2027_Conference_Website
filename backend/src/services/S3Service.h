#include<aws/s3/S3Client.h>
#include<aws/s3/model/GetObjectRequest.h>
#include<aws/s3/model/GetObjectResult.h>
#include<aws/s3/model/PutObjectRequest.h>
#include<aws/s3/model/PutObjectResult.h>
#include<aws/s3/model/DeleteObjectRequest.h>
#include<aws/s3/model/DeleteObjectResult.h>
#include<drogon/HttpAppFramework.h>

std::string getSignedUrl(const std::string& key); // -> returns signed url
std::string putObject(const std::string& key, const drogon::HttpFile& file); // -> returns put object key
bool deleteObject(const std::string& key); // -> returns true if object is deleted successfully, false otherwise
std::string urlToKey(const std::string& url); // -> extracts the object key from the given S3 URL. For example, if the URL is "https://mybucket.s3.amazonaws.com/path/to/object.jpg", it returns "path/to/object.jpg".
void shutdownS3Client(); // -> releases shared S3 client resources before Aws::ShutdownAPI