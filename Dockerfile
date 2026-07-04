# ==========================================
# STAGE 1: Build the C++ Backend Core
# ==========================================
FROM alpine:3.19 AS cpp-builder
RUN apk add --no-cache g++ make

WORKDIR /build
COPY MiniRedis/ .

# Compile only the production server binary
RUN make app

# ==========================================
# STAGE 2: Construct the Runtime Container
# ==========================================
FROM node:20-alpine AS runtime
RUN apk add --no-cache libstdc++

WORKDIR /app

# Copy compiled artifacts from Stage 1
COPY --from=cpp-builder /build/app ./mini-redis-server
# Initialize persistent storage footprints
RUN touch data.log snapshot.rdb

# Copy Node.js Web Dashboard Assets
COPY web-dashboard/package.json ./
COPY web-dashboard/server.js ./
COPY web-dashboard/public/ ./public/

# Configure Environment Variables for Production
ENV DASHBOARD_HOST=0.0.0.0
ENV MINI_REDIS_HOST=127.0.0.1
ENV MINI_REDIS_PORT=8080

# Expose Render's dynamic web routing port
EXPOSE 5173

# Execute both systems sequentially inside the container instance
CMD ./mini-redis-server & node server.js