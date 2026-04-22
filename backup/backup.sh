#!/bin/bash

set -e

DATE=$(date +%F-%H-%M)
FILE="/data/db-$DATE.sql.gz"

echo "Starting backup at $DATE"

# -------------------------
# Postgres Backup
# -------------------------
PGPASSWORD=$POSTGRES_PASSWORD pg_dump \
  -h $POSTGRES_HOST \
  -U $POSTGRES_USER \
  $POSTGRES_DB | gzip > $FILE

# Upload to S3
export AWS_ACCESS_KEY_ID=$S3_ACCESS_KEY
export AWS_SECRET_ACCESS_KEY=$S3_SECRET_KEY

AWS_ARGS=""
if [ -n "$S3_ENDPOINT" ]; then
  AWS_ARGS="--endpoint-url=$S3_ENDPOINT"
fi

aws s3 cp $FILE s3://$S3_BUCKET/postgres/ $AWS_ARGS

# Keep only last 5 locally
ls -1t /data/db-*.sql.gz | tail -n +6 | xargs -r rm --

# Keep only last 5 on S3
aws s3 ls s3://$S3_BUCKET/postgres/ $AWS_ARGS | \
  sort -r | awk '{print $4}' | tail -n +6 | while read file; do
    aws s3 rm s3://$S3_BUCKET/postgres/$file $AWS_ARGS
done

# -------------------------
# MinIO Backup
# -------------------------
mc alias set local $MINIO_ENDPOINT $MINIO_ACCESS_KEY $MINIO_SECRET_KEY

MC_REMOTE_URL=${S3_ENDPOINT:-"https://s3.amazonaws.com"}
mc alias set remote $MC_REMOTE_URL $S3_ACCESS_KEY $S3_SECRET_KEY

mc mirror --overwrite local/$MINIO_BUCKET remote/$S3_BUCKET/minio

echo "Backup completed!"