function clockSet() {

    $("#clockset").val(1);

    $("#control_form").submit();

}

function clockReset() {

    $("#clockset").val(0);
}

$(window).on('load', function () {
    console.log('All assets are loaded');
    clockReset();
})