<?php
include "config/+connection.php";
$conn     = mysqli_connect($host, $user, $pass, $database);
if ($conn == false) {
  echo "no connection";
}
$y_tegangan     = mysqli_query($conn, 'SELECT Tegangan FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_arus         = mysqli_query($conn, 'SELECT Arus FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_daya_semu    = mysqli_query($conn, 'SELECT Daya_semu FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_frekuensi    = mysqli_query($conn, 'SELECT frekuensi FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_faktor_daya  = mysqli_query($conn, 'SELECT Faktor_daya FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_daya_aktif   = mysqli_query($conn, 'SELECT Daya_aktif FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_daya_reaktif = mysqli_query($conn, 'SELECT Daya_reaktif FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$y_energi       = mysqli_query($conn, 'SELECT Energi FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$x_waktu1       = mysqli_query($conn, 'SELECT log_time FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');
$x_waktu2       = mysqli_query($conn, 'SELECT log_time FROM (SELECT * FROM Data_listrik ORDER BY ID DESC LIMIT 25) Var1 ORDER BY ID ASC');

session_start();
if (empty($_SESSION['akses'])) {
  echo "
        <script>
        alert('Mohon untuk login terlebih dahulu');
        window.location.href='index.php';
        </script>
    ";
}

?>

<!DOCTYPE html>
<html>

<head>
  <title></title>
  <link href="//maxcdn.bootstrapcdn.com/font-awesome/4.2.0/css/font-awesome.min.css" rel="stylesheet">
  <link href="https://fonts.googleapis.com/css?family=Roboto" rel="stylesheet">
  <link rel="stylesheet" href="assets/css/dashboard(2).css">
  <link rel="stylesheet" href="assets/css/bootstrap.css">

  <!-- Add custom CSS here -->
  <link rel="stylesheet" href="assets/css/sb-admin.css">
  <link rel="stylesheet" href="assets/font-awesome/css/font-awesome.min.css">
  <script src="assets/js/Chart.js"></script>

</head>

<body>
  <div class="row">
    <div class="col-lg-12">
      <h3 class="align-left">DASHBOARD</h3>
      <ol class="breadcrumb">
        <li class="active"><i class="fa fa-home"></i> Dashboard <?php $_SESSION['akses']; ?></li>
      </ol>
    </div>
  </div>

  <!-- Line Chart -->
  <div class="panel-heading">
    <h3 class="panel-title">
      <center>DAYA INPUT
    </h3>
  </div>
  <div class="container">
    <canvas id="linechart(1)" width="200" height="100"></canvas>
  </div>

  <div class="panel-heading">
    <h3 class="panel-title">
      <center>DAYA OUTPUT
    </h3>
  </div>
  <div class="container">
    <canvas id="linechart(2)" width="200" height="100"></canvas>
  </div>

  <!-- Device Profile -->
  <section>
    <div class="media">
      <a class="pull-left" href="#">
        <img src="img/image12.jpg" style="height: 200px; width: 200px;" class="media-object" />
      </a>
      <div class="media-body">
        <h3 class="media-heading">Device Profile</h3>
        <p class="align-justify">This device is a device that can perform two different tasks. The first is monitoring input power and output power in a single phase power system. The second is to monitor electricity and improve power factor, if the power factor measurement in a single-phase electrical system is in the range below 0.75, this device will automatically correct the power factor. Power factor improvement is done by calculating how much power factor value is needed to reach a value of 0.85 (PLN Standard), then determining the value of the required capacitor. The data read by this device will then be sent to an online database and the data can be accessed through this web monitoring. This device is also equipped with a backup datalogger consisting of a micro SD card module and a DS3231 RTC module, if the device cannot connect to a wifi network or the wifi network suddenly disconnects.</p>
      </div>
    </div>
  </section>

  <!-- Footer -->
  <footer>
    <div class="container">
      <div class="row">
        <div class="col-lg-5 col-md-5 col-sm-4 col-xs-12">
          <ul class="adress">
            <span>Find Me</span>
            <div class="align-justify">
              <li>
                <p>If you are interested about the internet of things, electrical, or related fields, I would love to discuss it and you can reach me on:</p>
              </li>
            </div>
            <li>
              <p>+62853 3338 9189</p>
            </li>
          </ul>
        </div>
        <div class="col-lg-5 col-md-5 col-sm-4 col-xs-12">
          <ul class="social">
            <span>Social</span>
            <li>
              <a href="https://github.com/aditya98-dot"><i class="fa fa-github fa-2x"></i></a>
            </li>
            <li>
              <a href="https://www.linkedin.com/in/aditya-prtm98"><i class="fa fa-linkedin fa-2x"></i></a>
            </li>
            <li>
              <a href="mailto:adityapratama141198@gmail.com"><i class="fa fa-envelope fa-2x"></i></a>
            </li>
          </ul>
        </div>
      </div>
    </div>
  </footer>
</body>

</html>

<script>
  var ctx = document.getElementById("linechart(1)").getContext("2d");
  var data = {
    labels: [<?php while ($b = mysqli_fetch_array($x_waktu1)) {
                echo '"' . $b['log_time'] . '",';
              } ?>],
    datasets: [{
        label: "Tegangan",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#29B0D0",
        borderColor: "#29B0D0",
        pointHoverBackgroundColor: "#29B0D0",
        pointHoverBorderColor: "#29B0D0",
        data: [<?php while ($b = mysqli_fetch_array($y_tegangan)) {
                  echo  $b['Tegangan'] . ',';
                } ?>]
      },
      {
        label: "Arus",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#DC143C",
        borderColor: "#DC143C",
        pointHoverBackgroundColor: "#DC143C",
        pointHoverBorderColor: "#DC143C",
        data: [<?php while ($b = mysqli_fetch_array($y_arus)) {
                  echo  $b['Arus'] . ',';
                } ?>]
      },
      {
        label: "Daya Semu",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#F07124",
        borderColor: "#F07124",
        pointHoverBackgroundColor: "#F07124",
        pointHoverBorderColor: "#F07124",
        data: [<?php while ($b = mysqli_fetch_array($y_daya_semu)) {
                  echo  $b['Daya_semu'] . ',';
                } ?>]
      },
      {
        label: "Frekuensi",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#00FF7F",
        borderColor: "#00FF7F",
        pointHoverBackgroundColor: "#00FF7F",
        pointHoverBorderColor: "#00FF7F",
        data: [<?php while ($b = mysqli_fetch_array($y_frekuensi)) {
                  echo  $b['Frekuensi'] . ',';
                } ?>]
      }
    ]
  };

  var myBarChart = new Chart(ctx, {
    type: 'line',
    data: data,
    options: {
      legend: {
        display: true
      },
      barValueSpacing: 20,
      scales: {
        yAxes: [{
          ticks: {
            min: 0,
          }
        }],
        xAxes: [{
          gridLines: {
            color: "rgba(0, 0, 0, 0)",
          }
        }]
      }
    }
  });

  var ctx = document.getElementById("linechart(2)").getContext("2d");
  var data = {
    labels: [<?php while ($b = mysqli_fetch_array($x_waktu2)) {
                echo '"' . $b['log_time'] . '",';
              } ?>],
    datasets: [{
        label: "Faktor Daya",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#00FF7F",
        borderColor: "#00FF7F",
        pointHoverBackgroundColor: "#00FF7F",
        pointHoverBorderColor: "#00FF7F",
        data: [<?php while ($b = mysqli_fetch_array($y_faktor_daya)) {
                  echo  $b['Faktor_daya'] . ',';
                } ?>]
      },
      {
        label: "Daya Aktif",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#FFD700",
        borderColor: "#FFD700",
        pointHoverBackgroundColor: "#FFD700",
        pointHoverBorderColor: "#FFD700",
        data: [<?php while ($b = mysqli_fetch_array($y_daya_aktif)) {
                  echo  $b['Daya_aktif'] . ',';
                } ?>]
      },
      {
        label: "Daya Reaktif",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#696969",
        borderColor: "#696969",
        pointHoverBackgroundColor: "#696969",
        pointHoverBorderColor: "#696969",
        data: [<?php while ($b = mysqli_fetch_array($y_daya_reaktif)) {
                  echo  $b['Daya_reaktif'] . ',';
                } ?>]
      },
      {
        label: "Energi",
        fill: false,
        lineTension: 0.2,
        backgroundColor: "#DC143C",
        borderColor: "#DC143C",
        pointHoverBackgroundColor: "#DC143C",
        pointHoverBorderColor: "#DC143C",
        data: [<?php while ($b = mysqli_fetch_array($y_energi)) {
                  echo  $b['Energi'] . ',';
                } ?>]
      }
    ]
  };

  var myBarChart = new Chart(ctx, {
    type: 'line',
    data: data,
    options: {
      legend: {
        display: true
      },
      barValueSpacing: 20,
      scales: {
        yAxes: [{
          ticks: {
            min: 0,
          }
        }],
        xAxes: [{
          gridLines: {
            color: "rgba(0, 0, 0, 0)",
          }
        }]
      }
    }
  });
</script>