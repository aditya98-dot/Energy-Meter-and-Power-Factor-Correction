<?php
$conn=mysqli_connect($host, $user, $pass, $database);
if($conn==false){
  echo"no connection";
}
?>
<!-- JavaScript -->
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.0/jquery.min.js"></script>
<script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/dataTables.buttons.min.js"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.flash.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.1.3/jszip.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/pdfmake.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/vfs_fonts.js"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.html5.min.js"></script>
<script src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.print.min.js"></script>

<!-- CSS -->
<link rel="stylesheet" href="https://cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css">
<link rel="stylesheet" href="https://cdn.datatables.net/buttons/1.5.6/css/buttons.dataTables.min.css">
<body>
  <div class="row">
    <div class="col-lg-12">
      <h1>Tabel Monitoring Listrik</h1>
      <ol class="breadcrumb">
        <li><a href=""><i class="fa fa-home"></i></a></li>
        <li class="active"><i class="fa fa-table"></i> Tabel Listrik</li>
      </ol>
    </div>
  </div>
<!-- Filter Tanggal -->
  <div class="row">
  	<div class="col-lg-12">
        <div class="table-responsive">
            <form method="post" class="form-inline">
                <div class="form-group">Dari Tanggal</div>
                <input type="date" name="tgl_mulai" class="form-control">
                <div class="form-group">Sampai Tanggal</div>
                <input type="date" name="tgl_selesai" class="form-control ml-3">
                <button type="submit" name="filter_tgl" class="btn btn-info ml-3">Filter</button>
            </form>
            <br>
            <!-- Export Data -->
                <script type="text/javascript"> 
                    $(document).ready(function () {
                        $('#datatables').DataTable({
                            scrollX: true,
                            oriented: 'landscape',
                            dom: "<'row'<'col-sm-12 col-md-4'l><'col-sm-12 col-md-4'B><'col-sm-12 col-md-4'f>>" +
                        "<'row'<'col-sm-12'tr>>" +
                        "<'row'<'col-sm-12 col-md-5'i><'col-sm-12 col-md-7'p>>",
                            buttons: ['copy', 'excel', 'print']
                        });
                    });
                </script>
            <br>
            <table class="table table-bordered table-hover table-striped" id="datatables">
                <thead>
                    <tr id="namatable">
                        <th>No.</th>
                        <th align='center'>Tanggal & Waktu</th>
                        <th>Tegangan (V)</th>
                        <th>Arus Listrik (A)</th>
                        <th>Daya Semu (VA)</th>
                        <th>Faktor Daya</th>
                        <th>Daya Aktif (Watt)</th>
                        <th>Daya Reaktif (Var)</th>
                        <th>Frekuensi (Hz)</th>
                        <th>Energi (kWh)</th>
                    </tr>
                </thead>
                <tbody id="responsecontainer">
                  <?php
                  $no = 1;

                  if (isset($_POST['filter_tgl'])){

                    $mulai = $_POST['tgl_mulai'];
                    $selesai = $_POST['tgl_selesai'];

                    if($mulai!=null || $selesai!=null){
                        $query = mysqli_query($conn, "SELECT * FROM Data_listrik WHERE log_time BETWEEN '$mulai' AND date_add('$selesai', INTERVAL 1 DAY) order by ID DESC");
                    } else {
                        $query = mysqli_query($conn, "SELECT * FROM Data_listrik");
                    }
                    
                  } else {
                    $query = mysqli_query($conn, "SELECT * FROM Data_listrik");
                  }
                  while($data = mysqli_fetch_array($query)){
                    echo"
                        <tr>
                            <td align='center'>$no</td>
                            <td>$data[log_time]</td>
                            <td align='center'>$data[Tegangan]</td> 
                            <td align='center'>$data[Arus]</td>
                            <td align='center'>$data[Daya_semu]</td>
                            <td align='center'>$data[Faktor_daya]</td>
                            <td align='center'>$data[Daya_aktif]</td>
                            <td align='center'>$data[Daya_reaktif]</td>
                            <td align='center'>$data[Frekuensi]</td>
                            <td align='center'>$data[Energi]</td>
                        </tr>
                    ";
                      $no++;
                    } 
                  ?>
                </tbody>
            </table>
        </div>
  </div>
</body>

