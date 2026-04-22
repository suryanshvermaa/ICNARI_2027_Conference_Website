FROM alpine:3.20

# Install required packages
RUN apk add --no-cache \
    bash \
    curl \
    ca-certificates \
    postgresql-client \
    aws-cli \
    tzdata

# Install MinIO client
RUN curl -O https://dl.min.io/client/mc/release/linux-amd64/mc && \
    chmod +x mc && mv mc /usr/local/bin/

WORKDIR /scripts

COPY backup.sh .
COPY start.sh .

RUN chmod +x backup.sh start.sh

CMD ["/scripts/start.sh"]