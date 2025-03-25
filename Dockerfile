# Use an official Ubuntu image
FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    git \
    libboost-all-dev

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Build the application
RUN cmake -B build && cmake --build build

# Expose the port
EXPOSE 5000

# Run the server
CMD ["./build/server"]
