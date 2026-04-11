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
                    ENV_FILE="/home/suryansh/env/icnari27-site/app.env"
                    if [ ! -f "$ENV_FILE" ]; then
                      echo "Missing env file at: $ENV_FILE"
                      echo "Directory listing (best-effort):"
                      ls -la "/home/suryansh/env/icnari27-site" || true
                      exit 1
                    fi
                    cp "$ENV_FILE" "$WORKSPACE/backend/.env"
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