cd docker-server-run
docker build --no-cache -t docker-server .
docker run -p 8080:80 docker-server

