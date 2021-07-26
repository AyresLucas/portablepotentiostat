// Rover App and rover training function table
//
// R.App              R.Training            Key      KeyCode    ; //previousKey
// move_forward()     fowardRover()         W        87 ; //W
// move_right()       rightTurnRover()      D        68 ; //D
// move_left()        leftTurnRover()       A        65 ; //A
// move_backward()    reverseRover()        S        83 ; //S
// stop_motor()       turnOffRover()        --       -- ; //spacebar DEPRICATED
// motor_on()         --                    --       -- ; //1
// motor_off()        --                    --       -- ; //2
// up_cam()           rotateMainCamBox( 1)  UP       38 ; //up
// down_cam()         rotateMainCamBox(-1)  DOWN     40 ; //down
// right_cam()        rotateMainCamTube(-1) RIGHT    39 ; //right
// left_cam()         rotateMainCamTube( 1) LEFT     37 ; //left
// led()              toggleLantern()       L        76 ; //L
// home_cam()         resetMainCam()        CONTROL  17 ; //control
// bracop()           rotateArm(-1)         G        71 ; //Y
// bracom()           rotateArm( 1)         T        84 ; //T
// antebracop()       rotateForeArm(-1)     H        72 ; //H
// antebracom()       rotateForeArm( 1)     Y        89 ; //G
// rotp()             rotateShoulder( 1)    J        74 ; //N
// rotm()             rotateShoulder(-1)    F        70 ; //B
// pincap()           rotateClaw(-1)        C        67 ; //. or >   (ABRIR)
// pincm()            rotateClaw( 1)        M        77 ; //, or <   (FECHAR)
// abre_porta()       --                    ??       -- ; //E DEPRICATED
// fecha_porta()      collectSample()       Enter    13 ; //R Kawai's funciton
//----- pinca_on and pinca_off will be merged in a toggle function.
// pinca_On_Off()     ToggleRoboticArm()    O        79 ; //O and P MERGED pinca_on and pinca_off
//-----
// pinca_home()       resetRoboticArm()     P        80 ; //I
// modo_linear()      --                    --       -- ; //5 DEPRICATED

document.addEventListener('keyup', function (event) {
    if (event.keyCode == 87) { //W
    stop_motor();
    }
    if (event.keyCode == 68) { //D
    stop_motor();
    }
    if (event.keyCode == 65) { //A
    stop_motor();
    }
    if (event.keyCode == 83) { //S
    stop_motor();
    event.preventDefault();
    }
});

document.addEventListener('keydown', function (event) {

    event.preventDefault();

    //if (!["labPage","labPick"].includes(currentPage)){
        
        if (event.keyCode == 87) { //W
            move_forward();
        }
        if (event.keyCode == 68) { //D
            move_right();
        }
        if (event.keyCode == 65) { //A
            move_left();
        }
        if (event.keyCode == 83) { //S
            move_backward();
        }
        // if (event.keyCode == 32) {
        //     stop_motor();
        // }
        // if (event.keyCode == 49) {
        //     motor_on();
        // }
        // if (event.keyCode == 50) {
        //     motor_off();
        // }
        if (event.keyCode == 38) { //UP ARROW
            up_cam();
        }
        if (event.keyCode == 40) { //DOWN ARROW
            down_cam();
        }
        if (event.keyCode == 39) { //RIGHT ARROW
            right_cam();
        
        }
        if (event.keyCode == 37) { //LEFT ARROW
            left_cam();
        
        }
        if (event.keyCode == 76) { //L
            led();
        }
        if (event.keyCode == 17) { //CONTROL
            home_cam();
        }
        if (event.keyCode == 71) { //G
            bracop();
        }
        if (event.keyCode == 84) { //T
            bracom();
        }
        if (event.keyCode == 72) { //H
            antebracop();
        }
        if (event.keyCode == 89) { //Y
            antebracom();
        }
        if (event.keyCode == 74) { //J
            rotp();
        }
        if (event.keyCode == 70) { //F
            rotm();
        }
        if (event.keyCode == 67) { //C
            pincap();
        }
        if (event.keyCode == 77) { //M
            pincm();
        }
        // if (event.keyCode == 69) {
        //     abre_porta();
        // }
        if (event.keyCode == 13) { //Enter
            fecha_porta();
        }
        if (event.keyCode == 79) { //O
            pinca_On_Off();
        }
        if (event.keyCode == 80) { //P
            pinca_home();
        }
        if (event.keyCode == 9) { //P
            toggleKeyMap();
        }
        event.preventDefault();
        // if (event.keyCode == 53) {
        //     modo_linear();
        // }
    //}

}, true);


keyMapDisplayed=false
function toggleKeyMap() {
  if (keyMapDisplayed){
    keyMapDisplayed=false
    document.getElementById("keyMap").style.display="none"
  }else{
    keyMapDisplayed=true
    document.getElementById("keyMap").style.display="block"

  }
}

setInterval("keep_alive()", 45000);// funcao secreta

function keep_alive() {
    sendMsg("998")
    return false;
}


