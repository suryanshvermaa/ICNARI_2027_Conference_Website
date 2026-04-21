pipeline{
    agent any
    stages{
        stage("Workspace Cleanup"){
            steps{
                script{
                    cleanWs()
                }
            }
        }
        stage("cloning"){
            steps{
                echo "cloning code..."
                git url: 'https://github.com/suryanshvermaa/ICNARI_2027_Conference_Website.git',branch: 'main'
                echo "cloning code successful."
            }
        }
        stage("print root paths"){
            steps{
                echo "Jenkins WORKSPACE: ${env.WORKSPACE}"
                echo "Jenkins HOME: ${env.HOME}"
                sh 'pwd && ls -la'
            }
        }
        stage("copy env file"){
            steps{
                echo "copying env file..."
                sh '''
                    set -e
                    APP_ENV_FILE="/home/suryansh/env/icnari27-site/app.env"
                    DB_ENV_FILE="/home/suryansh/env/icnari27-site/db.env"
                    S3_ENV_FILE="/home/suryansh/env/icnari27-site/s3.env"
                    if [ ! -f "$APP_ENV_FILE" ]; then
                      echo "Missing env file at: $APP_ENV_FILE"
                      echo "Directory listing (best-effort):"
                      ls -la "/home/suryansh/env/icnari27-site" || true
                      exit 1
                    fi
                    cp "$APP_ENV_FILE" "$WORKSPACE/backend/.env"
                    cp "$DB_ENV_FILE" "$WORKSPACE/backend/db.env"
                    cp "$S3_ENV_FILE" "$WORKSPACE/backend/s3.env"
                '''
                echo "copying env file successful."
            }
        }
        stage("copy config file"){
            steps{
                echo "copying config file..."
                sh '''
                    set -e
                    CONFIG_FILE="/home/suryansh/env/icnari27-site/config.json"
                    if [ ! -f "$CONFIG_FILE" ]; then
                      echo "Missing config file at: $CONFIG_FILE"
                      echo "Directory listing (best-effort):"
                      ls -la "/home/suryansh/env/icnari27-site" || true
                      exit 1
                    fi
                    cp "$CONFIG_FILE" "$WORKSPACE/backend/config/config.json"
                '''
                echo "copying config file successful."
            }
        }
        stage("deploying minio"){
            steps{
                dir('backend'){
                    echo "deploying minio..."
                    sh '''
                        set -e
                        if ! docker ps --format '{{.Names}}' | grep -q "icnari27_minio_container"; then
                          docker compose -f docker-compose.prod.s3.yml up -d
                        else
                          echo "minio is already running."
                        fi
                    '''
                    echo "minio deployed successfully."
                }
            }
        }
        stage("deploying postgres"){
            steps{
                dir('backend'){
                    echo "deploying postgres..."
                    sh '''
                        set -e
                        if ! docker ps --format '{{.Names}}' | grep -q "icnari27_db_container"; then
                          docker compose -f docker-compose.prod.db.yml up -d
                        else
                          echo "postgres is already running."
                        fi
                    '''
                    echo "postgres deployed successfully."
                }
            }
        }
        stage("deploying"){
            steps{
                dir('backend'){
                    echo "deploying application..."
                    sh '''
                        set -e
                        docker rm -f icnari27_app_container 2>/dev/null || true
                        docker compose -f docker-compose.prod.yml up -d --build --force-recreate
                    '''
                    echo "application deployed successfully."
                }
            }
        }
    }
}