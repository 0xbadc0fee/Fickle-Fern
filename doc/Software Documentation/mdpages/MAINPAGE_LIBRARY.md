# Flory 8772 Harvester Project Setup{#mainpage}

Simple overview of use/purpose.

The Flory 8772 control software is an embedded application developed for STW ESX controllers.  
It provides control of propulsion, engine systems, hydraulic subsystems, and auxiliary machine functions using a modular, function-based architecture.

---

### Author
STW Technic, LP © 2001-2026

## Description:

The Flory 8772 software is structured as a collection of independent control modules organized by machine function.  
Each module executes within a cyclic loop (~10 ms) and follows a standardized `init_*()` and `update_*()` pattern.

The system integrates:
- J1939 CAN communication for engine and display interfaces
- Hardware abstraction layers for inputs/outputs
- Shared helper functions for control logic (PID, ramp, filtering)

Each function includes:
- Core functionality logic
- Fault detection and handling behavior
- Requirement traceability (FR / IR / VS)

### Setup

* Target hardware: STW ESX controller family (ESX-4CM, ESX-4CL)
* Development environment: Logicad (Eclipse-based)
* Toolchain: HighTec TriCore compiler (GNU99)
* CAN stack: openSYDE (J1939 configuration)
* OS: Windows 10 or later

### Installing

* Clone or download the project repository
* Import project into Logicad workspace
* Ensure project is configured for the correct STW ESX target
* Verify HighTec TriCore toolchain is configured
* Build project using Logicad build configuration (Debug/Release)
* Download application to controller via CAN or service tool

## Example API Usage:

### Customize Public Header(s):

example.h

    typedef struct
    {
        float32 f32_target_value;
        float32 f32_actual_value;
    } T_ExampleModule;

### Implementation:

main.c

    int main(void)
    {
        init_exampleModule();

        while(1)
        {
            update_exampleModule();
        }
    }

## Sub Topics

### System Modules
- @subpage agvwork_functions "AgvWork Functions (1–11)"
- @subpage agvchassis_functions "AgvChassis Functions (12–16)"

### Shared Logic
- @subpage helper_functions "Helper Functions (17–22)"

### Auxiliary
- @subpage miscellaneous_functions "Miscellaneous (23)"

## Common Issues / Notes

* Ensure all I/O signal names match configured hardware mappings
* Verify CAN communication (J1939 PGNs) is properly configured in openSYDE
* Do not modify generated code under `/opensyde`
* Ensure cyclic timing (~10 ms) is maintained for stable control behavior

Example build flow:

    Clean -> Build -> Download

## References:

* STW internal development guidelines
* SAE J1939 CAN specification
* openSYDE documentation

## Change History

* 1.0
    * Initial implementation of function-based architecture
    * Added AgvWork, AgvChassis, and helper modules

* 0.1
    * Initial project setup

## License

Confidential - STW Technic  
Use permitted only under contract.