# Dotx Wind Turbine Controller (DWTC)
## Introduction
The Dotx Wind Turbine Controller is an certification-ready controller that is currently used in various types of wind turbines in the field. It minises both fatigue and ultimate loads, while optimizing power output.

The benefits of this controller are:
* Optimal Power (Power-Velocity curve)
* Fully coded in ANSI-C, which means it is very easy to deploy on a PLC. Also this allows companies to use git or SVN to work on the software in a team, without chaos.
* Fully tested in the field and in commercial wind turbines.

![DWTC Structure](http://dotxcontrol.com/img/DWTC_Structure.JPG)


## Controller details
The controller has different control modules/features
which can be switched on or off, among which:
* Optimal power and generator speed control
* Individual Pitch Control (0P, 1P and 2P) for
reducing loads on the nacelle and blades
* Active tower motion damping to reduce tower
fatigue
* Active drive train damping to reduce fatigue of the
drive train
* Finite-State Machine based supervisory control
* Extreme wind gust detection and control for
ultimate load reduction
* Model based fault detection (reducing ultimate
loads)
* Additional control features: DotX continues to
develop new control features to reduce both DEFL
and extreme loads, and improve power production.
Please check out our website for an overview of
the current ones, and their effect on loads and
performance. 

## Tuning Features
Several tools are available for tuning:
* Automatic generation of initial tuning settings
* Offline data analysis tools for fine-tuning, such as FFT
* Pre-defined notch filters, low pass filters, etc.
* Optional logging of all internal controller states and signals for improved data analysis
* Automatic test signal generation for wind turbines in the field, to compare field tests with model
tests



## Compatibility
The C-code can be compiled for any target. Known compatible PLC's are
* Bachmann
* MITA-Technik
* ...

## Compilation
In order to compile the code into a dll on a Windows CPU, please follow the following steps:

1. Install Minimalist GNU for Windows (MINGW). http://www.mingw.org
2. For compilation using Makefile, open a MinGW shell and type 
    $ build_mcu.bat 

3. For compilation using Eclipse ..

## Testing
To test the controller, please install the [Wind Turbine Controller Demo](http://dotxcontrol.com/wind-turbine-control) which is set up to run FAST with the DWTC .dll file. Please check [this](http://dotxcontrol.com/files/papers/wind_turbine_controller_demo.pdf) documentation on how to run the Wind Turbine Controller Demo.

[![Watch the video](http://dotxcontrol.com/img/video_placeholder.JPG)](https://youtu.be/5k_lySsV8Yc)

## Documentation
...


## License

The DWTC is distributed under the [GNU General Public License v3.0](http://en.wikipedia.org/wiki/GNU_General_Public_License).


 
 

