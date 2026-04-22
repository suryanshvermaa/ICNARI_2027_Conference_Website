#!/bin/sh

echo "Starting cron..."

# Export environment variables for cron
env > /etc/environment
chmod +x /etc/environment

# Run daily at 2:34 PM (or whatever time you set)
# We source /etc/environment to ensure the script has all Docker env vars
# We redirect to /proc/1/fd/1 to see logs in the docker compose output
echo "2 15 * * * . /etc/environment; /scripts/backup.sh > /proc/1/fd/1 2>&1" > /etc/crontabs/root 

crond -f