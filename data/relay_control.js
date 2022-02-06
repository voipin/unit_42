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




});
