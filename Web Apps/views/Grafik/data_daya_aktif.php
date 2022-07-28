<?php
include "../../config/+connection.php";
$conn = mysqli_connect($host, $user, $pass, $database);
if($conn==false){
  echo"no connection";
}
  $x_waktu  = mysqli_query($conn, 'SELECT log_time FROM ( SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
  $y_Daya_Aktif = mysqli_query($conn, 'SELECT Daya_aktif FROM ( SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC'); 
  ?>

  <div class="panel panel-primary">
    <div class="panel-heading">
      <h3 class="panel-title"><center>GRAFIK DAYA AKTIF (Watt) </h3>
    </div>

    <div class="panel-body">
      <canvas id="myChart"></canvas>
      <script>
       var canvas = document.getElementById('myChart');
        var data = {
            labels: [<?php while ($b = mysqli_fetch_array($x_waktu)) { echo '"' . $b['log_time'] . '",';}?>],
            datasets: [
            {
                label: "Daya Aktif", 
                fill: true,
                lineTension: 0.2,
                backgroundColor: "rgba(255, 254, 182, .2)",
                borderColor: "rgba(255, 240, 0, 1)",
                borderCapStyle: 'butt',
                borderDash: [],
                borderDashOffset: 0.0,
                borderJoinStyle: 'miter',
                pointBorderColor: "rgba(255, 240, 0, 1)",
                pointBackgroundColor: "#fff",
                pointBorderWidth: 1,
                pointHoverRadius: 5,
                pointHoverBackgroundColor: "rgba(255, 240, 0, 1)",
                pointHoverBorderColor: "rgba(255, 240, 0, 1)",
                pointHoverBorderWidth: 2,
                pointRadius: 5,
                pointHitRadius: 10,
                data: [<?php while ($b = mysqli_fetch_array($y_Daya_Aktif)) { echo  $b['Daya_aktif'] . ',';}?>],
            }
            ]
        };

        var option = 
        {
          showLines: true,
          animation: {duration: 0}
        };
        
        var myLineChart = Chart.Line(canvas,{
          data:data,
          options:option
        });

      </script>          
    </div>    
  </div>