$(document).on('click', '#btn2',function (event) {
var el = document.getElementById("name").value;
        $('#tablerows').empty();
        $("#content-loaded").empty(); 
        $.get(el, function(data) {
            var lines = data.split('\n');
            $.each(lines, function (n, elem) {
                var word = elem.split(' ');
                $('#tablerows').append('<tr>');
                $.each(word, function(n, elemrow) {
                $('#tablerows').append('<td>' + elemrow + '</td>');
                });
                $('#tablerows').append('</tr>');
                });
        });    
});

$(document).on('click', '#btn0',function (event) {
$('#content-loaded').empty();
    $("#content-loaded").append("File list:\n");
    $.get('list_files', function(data) {
        var lines = data.split('  ');
         $("#content-loaded").append(lines);
    });
    $('#content-loaded').show();
});

$(document).on('click', '#btn1',function (event) {
$('#content-loaded').empty();
$('#tablerows').empty();
});


function load_second_page(){
    $('#tablerows').empty();
    $("#content-loaded").empty();
    $("#content-loaded").load("second_page.html");

}



        
   
    

     
                  
