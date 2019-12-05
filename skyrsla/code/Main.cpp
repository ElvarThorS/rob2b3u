// To complete the VEXcode V5 Text project upgrade process, please follow the
// steps below.
// 
// 1. You can use the Robot Configuration window to recreate your V5 devices
//   - including any motors, sensors, 3-wire devices, and controllers.
// 
// 2. All previous code located in main.cpp has now been commented out. You
//   will need to migrate this code to the new "int main" structure created
//   below and keep in mind any new device names you may have set from the
//   Robot Configuration window. 
// 
// If you would like to go back to your original project, a complete backup
// of your original (pre-upgraded) project was created in a backup folder
// inside of this project's folder.

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

//#include "vex.h"

//using namespace vex;


 /*----------------------------------------------------------------------------*/
 /*                                                                            */
 /*    Module:       main.cpp                                                  */
 /*    Author:       VEX                                                       */
 /*    Created:      Mon Mar 25 2019                                           */
 /*    Description:  V5 project                                                */
 /*                                                                            */
 /*----------------------------------------------------------------------------*/
 // Drive Forward
 // This program instructs your robot to move forward at half power for three
 // seconds. There is a two second pause at the beginning of the program.
 //
 // Robot Configuration: 
 // [Smart Port]    [Name]        [Type]           [Description]       [Reversed]
 // Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
 // Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true
 //
 #include "vex.h"
 
 // ---- START VEXCODE CONFIGURED DEVICES ----
 // Robot Configuration:
 // [Name]               [Type]        [Port(s)]
 // ---- END VEXCODE CONFIGURED DEVICES ----
 
 using namespace vex;
 
 // A global instance of vex::brain used for printing to the V5 brain screen

 
 // define your global instances of motors and other devices here
 vex::vision::signature RED_TARGET = vex::vision::signature (1, 5503, 6079, 5790, -383, -159, -270, 5.1, 0);
 vex::motor LeftMotor   = vex::motor( vex::PORT11 );
 vex::motor RightMotor  = vex::motor( vex::PORT20, true );
 vex::motor RotateMotor   = vex::motor( vex::PORT19 );
 vex::motor AimMotor   = vex::motor( vex::PORT13 );
 vex::motor FireMotor   = vex::motor( vex::PORT17 );
 vex::controller Controller1 = vex::controller();
 vex::vision VisionSensor (vex::PORT12);
 
 bool InCombat = false;
 int TurnDir = 1;
 float FireMotorSpeed = 30;
 float AimMotorSpeed = 15;
 float RotateMotorSpeed = 5;
 double startTime;

 void Mode () 
 {
   
   if( InCombat == true) {InCombat = false; Brain.Screen.print("Combat Mode: False");Brain.Screen.newLine();}
   else{ InCombat = true; Brain.Screen.print(" Combat Mode: True");Brain.Screen.newLine();}
   
   return ;
 }
 
 void Reload()
 {
   AimMotor.setStopping(hold);
   Brain.Screen.clearScreen();
   Brain.Screen.print("hlaða");Brain.Screen.newLine();
   FireMotor.spin(vex::directionType::fwd, FireMotorSpeed, vex::velocityUnits::pct);
   vex::task::sleep(300);
   FireMotor.setStopping(hold);
   //this_thread::sleep_for(std::chrono::milliseconds(2000));
 }
 
 void Fire()
 {
   AimMotor.setStopping(hold);
   Brain.Screen.clearScreen();
   Brain.Screen.print("skjota");Brain.Screen.newLine();
   FireMotor.spin(vex::directionType::rev, FireMotorSpeed, vex::velocityUnits::pct);
   vex::task::sleep(300);
   FireMotor.stop();
   FireMotor.setStopping(hold);
   AimMotor.stop();
   AimMotor.setStopping(hold);
  // this_thread::sleep_for(std::chrono::milliseconds(2000));
  //s Brain.Screen.print("skotið");
 }

 void Turn(std::string dir)
 {
   if(dir == "Up")
   {
     AimMotor.spin(vex::directionType::rev, AimMotorSpeed*0.1, vex::velocityUnits::pct);
   }
   if(dir == "Down")
   {
     AimMotor.spin(vex::directionType::fwd, AimMotorSpeed, vex::velocityUnits::pct);
   }
   if(dir == "Left")
   {
     RotateMotor.spin(vex::directionType::rev, RotateMotorSpeed, vex::velocityUnits::pct);
   }
   if(dir == "Right")
   {
     RotateMotor.spin(vex::directionType::fwd, RotateMotorSpeed, vex::velocityUnits::pct);
   }
   if(dir == "Stop")
   {
     AimMotor.setStopping(hold);
     RotateMotor.stop();
   }
 }
 
 
 
 int main() {
     vexcodeInit();
     FireMotor.setStopping(hold);
     startTime = Brain.timer(sec);
     // Display that the program has started to the screen.
     Brain.Screen.print("Ballið er rétt að byrja");Brain.Screen.newLine();

     VisionSensor.setSignature(RED_TARGET);
 
     int CombatSpeed = 50;
     
     VisionSensor.setBrightness(50);
     int screen_middle_x = 316 / 2;
     int screen_middle_y = 212 / 2;
     bool linedup = false;
     
 
     //bool InCombat = false;
     
     // Use these variables to set the speed of the arm and claw.
   
     
     // Create an infinite loop so that the program can pull remote control values every iteration.
     // This loop causes the program to run forever.
     while(1) {
      Brain.Screen.setFont(mono40);
      Brain.Screen.clearLine(1, black);
      Brain.Screen.setCursor(Brain.Screen.row(), 1);
      Brain.Screen.setCursor(1, 1);
 
     while(InCombat == false)
     {
       LeftMotor.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
       //Controller1.ButtonY.pressed(Reload);
       //Controller1.ButtonR1.pressed(Fire);
       Controller1.ButtonA.pressed(Mode);
       while(not linedup) {
        //snap a picture
        VisionSensor.takeSnapshot(RED_TARGET);
        //did we see anything?
        if(VisionSensor.objectCount > 0) 
        {
            Brain.Screen.clearLine(1, black);
            Brain.Screen.print("Object Count: 1",VisionSensor.objectCount);
            //where was the largest thing?
            Controller1.ButtonA.pressed(Mode);
            if(VisionSensor.largestObject.centerX < screen_middle_x - 5) 
            {
                //on the left, turn left
                linedup = false;
                Turn("Left");
            }
            else if (VisionSensor.largestObject.centerX > screen_middle_x + 5) 
            {
              //on the right, turn right
              linedup = false;
              Turn("Right");
            }
            else if (VisionSensor.largestObject.centerY > screen_middle_y + 5)
            {
              //if below, aim up
              linedup = false;
              Turn("Up");
            }
            else if (VisionSensor.largestObject.centerY < screen_middle_y - 5)
            {
              //if above, aim down
              linedup = false;
              Turn("Down");
            }
            else 
            {
              //in the middle, we're done lining up
              Turn("Stop");
              Fire();
              Turn("Stop");
            }
        } 
        while(VisionSensor.objectCount == 0)
        {
          VisionSensor.takeSnapshot(RED_TARGET);
          Brain.Screen.clearLine(1, black);
          RotateMotor.rotateFor(90*TurnDir,deg,true);
          double currentTime = Brain.timer(sec) - startTime;
          Controller1.ButtonA.pressed(Mode);
          if(currentTime >8){
            startTime = Brain.timer(sec);
            TurnDir *= -1;
            Controller1.ButtonA.pressed(Mode);
          }
             
       
        }
    }
     }
 
     while(InCombat == true)
     {

     while(Controller1.Axis1.position() !=0)
     {
     RotateMotor.spin(vex::directionType::fwd, Controller1.Axis1.position(), vex::velocityUnits::pct);
     AimMotor.spin(vex::directionType::fwd, Controller1.Axis2.position()*0.1, vex::velocityUnits::pct);
     }
     while(Controller1.Axis2.position() !=0)
     {
     RotateMotor.spin(vex::directionType::fwd, Controller1.Axis1.position(), vex::velocityUnits::pct);
     AimMotor.spin(vex::directionType::fwd, Controller1.Axis2.position()*0.1, vex::velocityUnits::pct);
     }
 
     if(Controller1.Axis2.position() == 0)
     {
       AimMotor.setStopping(hold);
     }
     
     //N
     while(Controller1.ButtonUp.pressing())
     {
       LeftMotor.spin(vex::directionType::fwd, CombatSpeed, vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::fwd, CombatSpeed, vex::velocityUnits::pct);
     }
     //V
     while(Controller1.ButtonLeft.pressing())
     {
       LeftMotor.spin(vex::directionType::rev, CombatSpeed, vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::fwd, CombatSpeed, vex::velocityUnits::pct);
     }
     //S
     while(Controller1.ButtonDown.pressing())
     {
       LeftMotor.spin(vex::directionType::rev, CombatSpeed, vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::rev, CombatSpeed, vex::velocityUnits::pct);
     }
     //A
     while(Controller1.ButtonRight.pressing())
     {
       LeftMotor.spin(vex::directionType::fwd, CombatSpeed, vex::velocityUnits::pct);
       RightMotor.spin(vex::directionType::rev, CombatSpeed, vex::velocityUnits::pct);
     }
     
     if(Controller1.ButtonY.pressing() == true)
     {
       Reload();
     }
   

    if(Controller1.ButtonR1.pressing() == true)
     {
       Fire();
     }

     //Skiptir um Mode
     //Controller1.ButtonY.pressed(Reload);
     //Controller1.ButtonR1.pressed(Fire);
     Controller1.ButtonA.pressed(Mode);
 
     //Skjóta og hlaða
     //Controller1.ButtonB.pressed(Reload);
     //Controller1.ButtonR1.pressed(Fire);
 
     
 
     // stoppar alla motora
     AimMotor.stop();
 
     RotateMotor.stop();
 
     LeftMotor.stop();
 
     RightMotor.stop();
 
 
     }
 
 
     vex::task::sleep(20);
     }
 }
 
 
