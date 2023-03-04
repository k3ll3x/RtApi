# Base image
FROM rust:latest

# Install system dependencies
RUN apt-get update && \
    apt-get install -y python3 python3-pip

# Set the working directory to /app
WORKDIR /app

# Copy the Rust project files to the container
COPY . .

# Build the Rust project
RUN cargo build --release

# Expose the server port
EXPOSE 8000

# Run the server
CMD ["./target/release/gis-dashboard"]
