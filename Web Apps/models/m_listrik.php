<?php
class Listrik{

	private $mysqli;
	function __construct($conn) {
		$this->mysqli = $conn;
	}

	public function tampil($id = null) {
		$db = $this->mysqli->conn;
		$sql = "SELECT * FROM Data_listrik)";
		if($id != null){
			$sql .=" WHERE id_listrik = $id";
		}
		$query = $db->query($sql) or die ($db->error);
		return $query;
	}

	public function tampil_tgl($tgl1, $tgl2) {
		$db = $this->mysqli->conn;
		$sql = "SELECT * FROM Data_listrik WHERE log_time BETWEEN '$tgl1' AND '$tgl2'";
		$query = $db->query($sql) or die ($db->error);
		return $query;
	}
}

?>