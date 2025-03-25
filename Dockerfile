# Use an official Ubuntu image
FROM ubuntu:22.04

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    git \
    libboost-all-dev \
    libasio-dev

# Set the working directory inside the container
WORKDIR /app

# Copy the source code into the container
COPY . .

# Build the application
RUN cmake -B build && cmake --build build

# Expose the port your server is running on
EXPOSE 5000

# Run the server
CMD ["./build/server"]
