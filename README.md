# 🎓 ICNARI 2027 — NIT Patna Conference Website

Conference website + admin dashboard for **ICNARI 2027** (NIT Patna: Patna + Bihta campus).

This repository is a two-part app:

- **Frontend**: React + Vite (Chakra UI + Tailwind)
- **Backend**: C++ API using Drogon
- **Dependencies**: PostgreSQL (database) + MinIO (S3-compatible object storage)

## Repository layout

- `frontend/` — Vite + React app
- `backend/` — Drogon C++ API, SQL seed files, docker compose for local services

## Quick start (local development)

### 1) Start Postgres + MinIO

From the backend directory:

```bash
cd backend
docker compose up -d
```

This provisions:

- PostgreSQL on `localhost:5432` (db: `icnari_conference_db`, user: `postgres`, pass: `postgres`)
- MinIO S3 API on `http://localhost:9000`
- MinIO Console on `http://localhost:9001` (dev credentials: `suryansh` / `suryansh`)

The SQL files in `backend/sql/` are mounted into the Postgres init folder.

### 2) Configure the backend

The backend reads environment variables from a local `.env` file:

```bash
cd backend
cp .env.example .env
```

Common variables (see `backend/.env.example`):

- `PORT` (defaults to `3000`)
- `JWT_SECRET`
- `S3_ENDPOINT`, `S3_REGION`, `S3_ACCESS_KEY`, `S3_SECRET_KEY`, `S3_BUCKET`, `S3_USE_SSL`

Database connection settings are in `backend/config/config.json`.

### 3) Build + run the backend

Prerequisites:

- C++ toolchain (gcc/clang)
- CMake (>= 3.15)
- Conan 2.x

Build:

```bash
cd backend
conan profile detect --force
conan install . \
	--output-folder=build \
	--build=missing \
	-c tools.system.package_manager:mode=install \
	-c tools.system.package_manager:sudo=True

cmake --preset conan-release
cmake --build --preset conan-release
```

Run:

```bash
cd backend
./build/ICNARI_Conference_Backend
```

Important: the server loads `./config/config.json` via a relative path, so run it from inside `backend/`.

Optional: create an admin user (after building):

```bash
cd backend
./build/createAdmin ./config/config.json
```

### 4) Configure + run the frontend

Prerequisites:

- Node.js (LTS recommended)
- pnpm (recommended; repo includes `pnpm-lock.yaml`)

Configure API base URL:

```bash
cd frontend
cp .env.example .env
```

`frontend/.env.example` defines:

- `VITE_API_URL=http://localhost:3000`

Install + run:

```bash
cd frontend
pnpm install
pnpm dev
```

Vite typically serves at `http://localhost:5173`.

Note: the frontend currently calls endpoints like `${VITE_API_URL}/...` (for example, `.../contact`, `.../photogallery/...`). The C++ backend defines versioned routes under `/api/v1/...` (see API section below). If you see 404s, align the frontend endpoint paths with the backend routes (or update `VITE_API_URL` to include the correct base path if you add a proxy).

## API documentation

- OpenAPI spec: `backend/routes/openapi.yaml`
- Health check: `GET /health`
- Versioned routes: `/api/v1/...` (auth/users/notifications/gallery)

## Docker (backend container)

The backend includes a production compose file:

```bash
cd backend
docker compose -f docker-compose.prod.yml up --build
```

Notes:

- The container maps `3000:3000`; ensure `PORT=3000` in `backend/.env`.
- `backend/docker-compose.prod.yml` mounts `backend/config/` into `/app/config/` (read-only).
- The `backend/Dockerfile` declares `EXPOSE 8080`, but the app listens on `PORT` (default `3000`).

## Troubleshooting

- **Backend can’t find config**: run the binary from `backend/` so `./config/config.json` resolves.
- **S3 errors**: ensure `S3_BUCKET` is set; MinIO must be running.
- **DB connection failures**: confirm Postgres is running and `backend/config/config.json` matches your local DB.
