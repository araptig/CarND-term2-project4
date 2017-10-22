# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Overview & Implementation 

- I implemented the generic PID controller that controls the steering angle of a self driving car 
  - did not use velocity or angle information for the PID controller update
- I pushed the design by increasing the  throttle from 0.30 to 0.45
- I saturated the signal so that total error is between -1 and 1 



## Reflections & Analysis

### The effect of the P, I, D components in my implementation.

- The P is the starting point

  - the larger the coefficient Kp, the faster is the reaction time
  - Kp is the main knob to play with
  - larger Kp is needed for sharper turns
    - with small Kp I could not control sharp turns, irrespective of other parameters
  - however larger Kp also introduces overshoot and hence oscillation which is not desirable
  - the larger Kp, the harder I found to control the system overall
    - I ended up choosing the smallest Kp I get away with

- The D to suppress oscillations

  - I increased Kd to counter the overshoot/oscillations introduced by Kp
  - however, Kd>0 was not as effective as I hoped
  - specially, when I increased Kd beyond a certain value it became less stable, and more wobbly, and jittery 
    - Kd suppressed oscillations never really eliminated it
  - having said that, increasing Kd beyond zero was very helpful

- The I for addressing systematic bias

  - I did not observe much of a difference whether Ki was zero or positive
  - my guess is that the motion model did not introduce systematic bias and that is why I did not find it useful

  ​

### Choosing the final hyper-parameters

- I optimized the 3 parameters K =(Kp, Ki, Kd ) manually
- I started with Kp only, and set the other 2 parameters to zero. The throttle was set at default 0.3, 
  - for Kp < 0.1, there was not enough control on the car
  - at Kp = 0.1 the car had descent reaction time but was oscillating
- While keeping Kp= 0.1, I increased Kd slowly from 0 to 0.6 
  - at Kd=0.4 there were still overshoots and the car could not make the sharp turns
  - at Kd=0.6 the car was being reasonably controlled
- Then I increased the throttle setting from 0.3 to 0.45
  - with the same settings (Kp, Kd)=(0.1,0.6) now I was seeing both overshoots and failure to make sharp turns
  - observation: as throttle increased both Kp & Kd needed to increase
    - but it seemed like Kd needed to increase more than the Kp
  - I increased Kp to 0.11 where I could just make the sharp turns, then I increased Kd to manage the oscillations
    - I stopped Kd at 0.9
    - The system did not seem too sensitive to Kd, I could have increaded it more
  - I played with Ki but did not see observable difference
    - for the sake of robustness set Ki to a small number 0.0003
-  **In summary ended up with (Kp, Ki, Kd) = (0.11, 0.0003, 0.9) @throttle 0.45**
  - the car is a little wobbly but stays on the road



# Generic Read-me content:

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

