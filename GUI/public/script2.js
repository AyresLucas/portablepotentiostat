var status = 0;
var status2 = 0;


function ciclicvoltametry (){

  if (status==0){
    
    $("#boxcv").css("display", "block");
    $("#boxamp").css("display", "none");
     status = 1;
     return;
  }
  else {
    
    $("#boxcv").css("display", "none");
    
    status = 0;
    return;

  }
  
}




function amperometry (){

  if (status2==0){

    $("#boxamp").css("display", "block");
    $("#boxcv").css("display", "none");
     status2 = 1;
     return;
  }
  else {
  
    $("#boxamp").css("display", "none");
    status2 = 0;
    return;

  }
  
}

