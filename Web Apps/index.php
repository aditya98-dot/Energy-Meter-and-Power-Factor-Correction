<?php
session_start();
if (isset($_SESSION['akses'])) {
	header("location:struktur.php");
}
?>
<!DOCTYPE html>
<html>

<head>
	<link rel="stylesheet" type="text/css" href="assets/css/login.css">
	<title>Monitoring Listrik</title>
	<link rel="stylesheet" type="text/css" href="">
	<!-- <script type="text/javascript" href= ""/> -->
</head>

<body>
	<div id="utama">
		<div id="judul">
			Halaman Login
		</div>
		<div id="inputan">
			<form action="#>" method="post">
				<div>
					<input type="text" name="user" placeholder="Username" class="lg" />
				</div>
				<div style="margin-top: 10px">
					<input type="password" name="pass" placeholder="Password" class="lg" />
				</div>
				<div style="margin-top: 10px">
					<input type="submit" name="login" value="Login" class="btn" />
				</div>
			</form>
		</div>
	</div>
</body>

</html>

<?php
if (isset($_POST['login'])) {
	$user = $_POST['user'];
	$pass = $_POST['pass'];
	if ($user == 'elektro_udayana' and $pass == 'elektro2022') {
		$_SESSION['akses'] = $user;
		header("location:struktur.php");
	} else {
		echo "
				<script>
				alert('Maaf, Username dan Password salah!');
				window.location.href='index.php';
				</script>
			";
	}
}
?>