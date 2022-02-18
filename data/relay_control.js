$(document).ready(function () {

    $('#fan').change(function () {
        console.log("fan!");
        if (this.checked) {
            $("#fan_action").load("fan_relay_on")
            console.log("fan on");

        } else {
            $("#fan_action").load("fan_relay_off")
            console.log("fan off");
        }

    });

    $('#humidity').change(function () {
        console.log("humidity!");

        if (this.checked) {
            $("#humidity_action").load("humidity_relay_on")
            console.log("humidity on");
        } else {
            $("#humidity_action").load("humidity_relay_off")
            console.log("humidity off");
        }

    });

    $('#light').change(function () {
        console.log("light!");

        if (this.checked) {
            $("#light_action").load("light_relay_on")
            console.log("light on");
        } else {
            $("#light_action").load("light_relay_off")
            console.log("light off");
        }

    });
    
    $('#manual_override').change(function () {
        console.log("manual_override!");

        if (this.checked) {
            $("#manual_action").load("manual_override_on")
            console.log("light on");
        } else {
            $("#manual_action").load("manual_override_off")
            console.log("light off");
        }

    });



});
