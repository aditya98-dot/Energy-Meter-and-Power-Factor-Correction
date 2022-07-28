<?php
ob_start();
require_once('config/+connection.php');
require_once('models/database.php');

$connection = new Database($host, $user, $pass, $database);
?>
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta name="description" content="">
  <meta name="author" content="">
  <title>Monitoring Listrik</title>

  <!-- Bootstrap core CSS -->
  <link rel="stylesheet" href="assets/css/bootstrap.css">
  <link rel="stylesheet" href="assets/DataTables/datatables.min.css">

  <!-- Add custom CSS here -->
  <link rel="stylesheet" href="assets/css/sb-admin.css">
  <link rel="stylesheet" href="assets/font-awesome/css/font-awesome.min.css">
</head>

<body>
  <div id="wrapper">

    <!-- Sidebar -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
      <!-- Brand and toggle get grouped for better mobile display -->
      <div class="navbar-header">
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-ex1-collapse">
          <span class="sr-only">Toggle navigation</span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
          <span class="icon-bar"></span>
        </button>
        <a class="navbar-brand" href="">Monitoring Listrik Energi Listrik dan Perbaikan Faktor Daya</a>
      </div>

      <!-- Collect the nav links, forms, and other content for toggling -->
      <div class="collapse navbar-collapse navbar-ex1-collapse">
        <ul class="nav navbar-nav side-nav">
          <li><a href="?page=dashboard"><i class="fa fa-home"></i> Dashboard</a></li>
          <li><a href="?page=Tabel Listrik"><i class="fa fa-table"></i> Tables</a></li>
          <li class="dropdown">
            <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-bar-chart-o"></i> Input Graph<b class="caret"></b></a>
            <ul class="dropdown-menu">
              <li><a href="?page=Grafik Tegangan">Grafik Tegangan</a></li>
              <li><a href="?page=Grafik Arus Listrik">Grafik Arus Listrik</a></li>
              <li><a href="?page=Grafik Daya Semu">Grafik Daya Semu</a></li>
              <li><a href="?page=Grafik Frekuensi">Grafik Frekuensi</a></li>
            </ul>
          </li>
          <li class="dropdown">
            <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-bar-chart-o"></i> Output Graph<b class="caret"></b></a>
            <ul class="dropdown-menu">
              <li><a href="?page=Grafik Faktor Daya">Grafik Faktor Daya</a></li>
              <li><a href="?page=Grafik Daya Aktif">Grafik Daya Aktif</a></li>
              <li><a href="?page=Grafik Daya Reaktif">Grafik Daya Reaktif</a></li>
              <li><a href="?page=Grafik Energi">Grafik Energi Listrik</a></li>
            </ul>
          </li>
        </ul>

        <ul class="nav navbar-nav navbar-right navbar-user">
          <li class="dropdown user-dropdown">
            <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-user"></i> Admin <b class="caret"></b></a>
            <ul class="dropdown-menu">
              <li><a href="logout.php"><i class="fa fa-power-off"></i>Log Out</a></li>
            </ul>
          </li>
        </ul>
      </div>
      <!-- /.navbar-collapse -->
    </nav>

    <div id="page-wrapper">

      <?php
      if (@$_GET['page'] == 'dashboard' || @$_GET['page'] == '') {
        include "views/dashboard.php";
      } else if (@$_GET['page'] == 'Tabel Listrik') {
        include "views/tabel listrik.php";
      } else if (@$_GET['page'] == 'Grafik Tegangan') {
        include "views/Grafik/grafik_tegangan.php";
      } else if (@$_GET['page'] == 'Grafik Arus Listrik') {
        include "views/Grafik/grafik_arus.php";
      } else if (@$_GET['page'] == 'Grafik Daya Semu') {
        include "views/Grafik/grafik_daya_semu.php";
      } else if (@$_GET['page'] == 'Grafik Frekuensi') {
        include "views/Grafik/grafik_frekuensi.php";
      } else if (@$_GET['page'] == 'Grafik Faktor Daya') {
        include "views/Grafik/grafik_faktor_daya.php";
      } else if (@$_GET['page'] == 'Grafik Daya Aktif') {
        include "views/Grafik/grafik_daya_aktif.php";
      } else if (@$_GET['page'] == 'Grafik Daya Reaktif') {
        include "views/Grafik/grafik_daya_reaktif.php";
      } else if (@$_GET['page'] == 'Grafik Energi') {
        include "views/Grafik/grafik_energi.php";
      }
      ?>

    </div><!-- /#page-wrapper -->

  </div><!-- /#wrapper -->

  <!-- JavaScript -->
  <script src="assets/js/jquery-3.4.0.min.js"></script>
  <script src="assets/js/bootstrap.js"></script>
  <script src="assets/DataTables/datatables.min.js"></script>

</body>

</html>