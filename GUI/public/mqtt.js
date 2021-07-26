var connected_flag = 0
var mqtt;
var reconnectTimeout = 2000;
var host = "mqtt.eclipseprojects.io";
var port = 443;
var topic1 = "uLABHUB2021";
var topic2 = "LBACAMERA001";
var pincaOn = false;

var filter = 1;


function caculateMovingAverage(datal, window) {
    var steps = datal.length - window;
    var result = [];
    for (var i = 0; i < steps; ++i) {
        var limit = i + window;
        var sum = 0;
        for (var j = i; j < limit; ++j) {
            sum += datal[j];
        }
        result.push(sum / window);
    }
    return result;
}



function onConnectionLost() {
    document.getElementById("status").innerHTML = "OFFLINE";
    alert("Conexao perdida!");
    console.log("connection lost");
    document.getElementById("messages").innerHTML = "Conexao perdida";
    connected_flag = 0;


}

function onFailure(message) {
    console.log("Failed");
    document.getElementById("messages").innerHTML = "Connection Failed- Retrying";
    setTimeout(MQTTconnect, reconnectTimeout);
    //alert("Conexao perdida!");
}

function onMessageArrived(r_message) {
    //out_msg = "Mensagem do ROVER: " + r_message.payloadString ;
    //out_msg = out_msg + " Message received Topic " + r_message.destinationName; // apenas para mostrar o topico da mensagem 
    //r_message = str.split(";");
    //console.log(r_message[0]);

    console.log(r_message.payloadString); // print the raw value
    const data = r_message.payloadString;
    var [y, x] = data.split(";"); // splitting the variables from the potentiostat

    xlabels.push(x); // add income data to the x variable
    ylabels.push(y); // add income data to the y variable
    addData(); // plotting the graph

 
    //document.getElementById("messages").innerHTML = out_msg;

}

function onConnected(recon, url) {
    //console.log(" in onConnected " + reconn);node
}

function onConnect() {
    // Once a connection has been made, make a subscription and send a message.
    //document.getElementById("messages").innerHTML = "Connected to " + host + "on port " + port;
    connected_flag = 1
    document.getElementById("status").innerHTML = "ONLINE";
    mqtt.subscribe("pot_channel1_data");
    //message = new Paho.MQTT.Message("Hello World");
    //message.destinationName = "sensor1";
    //mqtt.send(message);
    return false;
}



function MQTTdisconnect() {

    mqtt.disconnect();
    document.getElementById("status").innerHTML = "OFFLINE";
    alert("Você está desconectado!");

}

function MQTTconnect() {



    console.log("connecting to " + host + " " + port);

    mqtt = new Paho.MQTT.Client(host, port, "zuyLyonHastagprioriamateratsuLOL");


    var options = {
        timeout: 20,
        useSSL: true,
        userName: "",
        password: "",
        onSuccess: onConnect,
        onFailure: onFailure,
    };

    mqtt.connect(options);
    mqtt.onConnectionLost = onConnectionLost;
    mqtt.onMessageArrived = onMessageArrived;
    mqtt.onConnected = onConnected;
    alert("Conexão estabelecida!")
    return false;


}

function sub_topics() {
    document.getElementById("messages").innerHTML = "";
    if (connected_flag == 0) {
        out_msg = "<b>Not Connected so can't subscribeI</b>"
        console.log(out_msg);
        document.getElementById("messages").innerHTML = out_msg;
        return false;
    }
    var stopic = document.forms["subs"]["Stopic"].value;
    console.log("Subscribing to topic =" + stopic);
    mqtt.subscribe(stopic);
    return false;
}




function noConnectionMsg() {
    out_msg = "Sem conexao";
    console.log(out_msg);
    document.getElementById("messages").innerHTML = out_msg;
}

function sendMsg(msg, topic = topic1) {
    message = new Paho.MQTT.Message(msg);
    message.destinationName = topic;
    mqtt.send(message);
}


function move_forward() {

    // Connection Check
    if (connected_flag == 0) {
        noConnectionMsg();
    } else {

        sendMsg("29");
    }
    return false
}



setInterval("keep_alive()", 45000);// funcao secreta

function keep_alive() {
    sendMsg("998")
    return false;
}

// Script responsible for plotting the graph in the main page.
// Variables: y (current in bits) and x (voltage in bits)



var xlabels = [];
var ylabels = [];

const data = {

    labels: xlabels,
    datasets: [{
        label: 'Channel 1',
        backgroundColor: 'orange',
        borderColor: 'orange',
        data: ylabels,
    }]


};

// </block:setup>

// <block:config:0>
const config = {
    type: 'line',
    data,

    options: {
          animation: false,  
          parsing: true,
          normalized: true,


        //scales: {
//x: {
  //              type: 'linear',
    //            min: 0,
      //          max: 10000
        //    },

        //},
      
        }
};


// </block:config>

module.exports = {
    actions: [],
    config: config,
};


var myChart = new Chart(
    document.getElementById('chart'),
    config
);

async function addData() {

    myChart.data.datasets[0].data = ylabels;
  


        myChart.update('none');
  



    return false
  ;
}

function startamp() {

    const potential = document.getElementById("ampForm").elements[0].value;
    const time = document.getElementById("ampForm").elements[1].value;
    const sampleinterval = document.getElementById("ampForm").elements[2].value;
    const amparray= [];
    amparray.push("0"); // indicator for amp
    amparray.push(potential);
    amparray.push(time);
    amparray.push(sampleinterval);
    //console.log(amparray[0,1,2]);
    let text = amparray.toString();
    console.log(text);
    sendMsg(text);
    return false
 
}


function startCV() {

    const e_init = document.getElementById("cvform").elements[0].value;
    const e_high = document.getElementById("cvform").elements[1].value;
    const e_low = document.getElementById("cvform").elements[2].value;
    const scanrate = document.getElementById("cvform").elements[3].value;
    const sweepsegments= document.getElementById("cvform").elements[4].value;
    const cvarray= [];
    cvarray.push("1"); // indicator for amp
    cvarray.push(e_init);
    cvarray.push(e_high);
    cvarray.push(e_low);
    cvarray.push(scanrate);
    cvarray.push(sweepsegments);
    //console.log(amparray[0,1,2]);
    let text = cvarray.toString();
    console.log(text);
    sendMsg(text);
    return false
 
}


