<script>
var refreshId = setInterval(function()
{
    $('#responsecontainer').load('./views/Grafik/data_daya_semu.php');
}, 1000);
</script>

<script src="jquery-latest.js"></script> 
<script type="text/javascript" src="assets/js/jquery-3.4.0.min.js"></script>
<script type="text/javascript" src="assets/js/mdb.min.js"></script>

<!DOCTYPE html>
<html lang="en">
<head>
</head>
<body>
  <div class="container">
      <div class="center" id="responsecontainer" style="width: 75%"></div>    
  </div>    
</body>
</html>

