# Electrium Mobility F25 Skateboard
This repository contains all the firmware and hardware for Electrium Mobility's F25 Electric Skateboard.

## Projects
- Telemetry
- Power Distribution
- Tail Lights
- Head Lights

## Setup

### Prerequisites
- Git (install and configure with GitHub account)

#### Firmware Subteam
- VS Code
- PlatformIO Extension (on VS Code)

#### Electrical Subteam
- KiCad 9 (latest)

### Clone the Repo
```bash
git clone https://github.com/leogmendoza/skateboard-f25.git
cd skateboard-f25
```

## Contributing

### Firmware Instructions
1. Open VS Code
2. Navigate to your assigned project's `fw` folder (e.g. telemetry):
    ```bash
    cd telemetry/fw
3. Develop, then use PlatformIO tools to compile, run, and view serial output: 
    - Build
    - Upload
    - Monitor

### Electrical Instructions 
1. Open KiCad 9
2. Open `.kicad_pro` file in your assigned project's `hw/<project>` folder (e.g. telemetry):
    ```bash
    telemetry/hw/telemetry/telemetry.kicad_pro
3. Edit schematic or PCB layout

## Git Workflow
1. Before starting development, create a branch for your task:
    ```bash
    git checkout -b <project>-<task>
2. Develop!
3. Stage your changes before committing:
    ```bash
    git add .
4. Make regular commits with clear messages to document changes (e.g. tail lights firmware):
    ```bash
    git commit -m "Defined light controller FSM"
5. Push your local changes to the remote branch:
    ```bash
    git push origin <project>-<task>
6. Open a pull request (PR) on GitHub and wait for it to be reviewed and merged!
