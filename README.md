# Backup and Production Server System

A system of servers designed to handle production and backup tasks.

## Table of Contents

- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Configuration and Installation](#configuration-and-installation)
- [Running the Application](#running-the-application)

## Technologies Used

- **Programming Language**: C
- **Libraries**: POSIX, Standard C Library

## Project Structure

Describe the overall structure of your project. For example:

- /ServeurDeProduction.c: Contains the source code for the Production Server.
- /ServeurDeBackup.c: Contains the source code for the Backup Server.
- /ServeurDIntegration.c: Contains the source code for the Integration Server.

## Configuration and Installation

### Prerequisites

- GCC (GNU Compiler Collection)

### Installation

#### Compile the Servers

1. Navigate to the project directory.
2. Compile the Production Server:
    ```sh
    gcc -o ServeurDeProduction ServeurDeProduction.c
    ```
3. Compile the Backup Server:
    ```sh
    gcc -o ServeurDeBackup ServeurDeBackup.c
    ```
4. Compile the Integration Server:
    ```sh
    gcc -o ServeurDIntegration ServeurDIntegration.c
    ```

## Running the Application

### Production Server

1. Start the Production Server:
    ```sh
    ./ServeurDeProduction
    ```

### Backup Server

1. Start the Backup Server:
    ```sh
    ./ServeurDeBackup
    ```

### Integration Server

1. Run the Integration Server:
    ```sh
    ./ServeurDIntegration
    ```

### Notes

- Ensure that the `ProductionData` and `BackupData` directories exist and contain the necessary files.
- The Integration Server includes a sample file synchronization call `copy_file("example.txt");`. Modify as needed.
- The Integration Server has a commented-out block for testing server availability. Enable as needed.

## Author

Mohamad Akoum