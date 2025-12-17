# Stage 1: Build the C++ Application
FROM debian:bullseye-slim AS cpp-builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy C++ source code
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/
COPY tests/ tests/

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Stage 2: Setup Node.js Server and Final Image
FROM node:18-bullseye-slim

# Install runtime dependencies for C++ app (if any shared libs are needed)
# standard C++ libs are usually present, but let's be safe
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled binary from Stage 1
COPY --from=cpp-builder /app/build/bin/msh /app/build/bin/msh
RUN chmod +x /app/build/bin/msh

# Copy Web Interface
COPY web/package.json ./web/
COPY web/server.js ./web/
COPY web/public ./web/public

# Install Node.js dependencies
WORKDIR /app/web
RUN npm install

# Expose port
EXPOSE 3000

# Start the server
CMD ["npm", "start"]
