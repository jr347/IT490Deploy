<?php

class deployDB {

private $deploydb;

public function __construct() {
	$this->deploydb = new mysqli('localhost', 'root', 'kamran', 'vcon' );
	if ($this->deploydb->connect_error){
	die('Connect Error (' . $this->deploydb->connect_errno . ')' . $this->deploydb->connect_error);
	}
	
	echo 'Success... ' . $this->deploydb->host_info . "\n";
}

public function getNextPackage($req){
	$name = $this->deploydb->real_escape_string($req['packageName']);
	if($name == 'FEpackage-v'){
		$packageInfo = "Select Max(ver) from fe";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'];
                        $in = "Insert into fe (name) values ('feqav-$next')";
                        $a = $this->deploydb->query($in);
			return $result['Max(ver)'];
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'];
                        $in = "Insert into fe (name) values ('feqav-$next')";
                        $a = $this->deploydb->query($in);
			return '1';
			}
	}
	elseif($name == 'BEpackage-v') {
		$packageInfo = "Select Max(ver) from be";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'];
                        $in = "Insert into be (name) values ('beqav-$next')";
                        $a = $this->deploydb->query($in);
			return $result['Max(ver)'];
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'];
                        $in = "Insert into be (name) values (beqav-$next])";
                        $a = $this->deploydb->query($in);
			return '1';
			}	
	}


}

public function getNum($req){
	$name = $this->deploydb->real_escape_string($req['packageName']);
	if($name == 'FEpackage-v'){
		$packageInfo = "Select Max(ver) from fe";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 2;
			return $next;
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			return '1';
			}
	}
	elseif($name == 'BEpackage-v') {
		$packageInfo = "Select Max(ver) from be";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] -2;
			return $next;
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			return '1';
			}	
	}


}

public function pNum($req){
	$name = $this->deploydb->real_escape_string($req['packageName']);
	if($name == 'FEpackage-v'){
		$packageInfo = "Select Max(ver) from fe";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 4;
			return $next;
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			return '1';
			}
	}
	elseif($name == 'BEpackage-v') {
		$packageInfo = "Select Max(ver) from be";
		$response = $this->deploydb->query($packageInfo);
		$count = $response->num_rows;
		if($count > 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] -4;
			return $next;
		}
		elseif($count == 0){
			$result = $response->fetch_array(MYSQLI_ASSOC);
			return '1';
			}	
	}


}

public function deployFile($req) {
	$data = $req['packageName'];
	$level = $req['level'];
	$fepackage = "Select Max(ver) from fe";
	$bepackage = "Select Max(ver) from be";
	if ($level == 'QA'){
		if($data == 'FEpackage-v'){
			$response = $this->deploydb->query($fepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 2;
			$send_file = shell_exec('scp feqav-'.$next.'.tar.gz johnny@192.168.43.61:/home/johnny/Desktop');
			$install_file = shell_exec('./installFEQA.php');
			return true;
		}
		elseif($data == 'BEpackage-v'){
			$response = $this->deploydb->query($bepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 2;
			$send_file = shell_exec('scp beqav-'.$next.'.tar.gz ethan@192.168.43.7:/home/ethan/Desktop');
			$install_file = shell_exec('./installBEQA.php');
			return true;
		}
				
	}
	elseif ($level == 'Prod'){
		if($data == 'FEpackage-v'){
			$response = $this->deploydb->query($fepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 2;
			$send_file = shell_exec('scp feqav-'.$next.'.tar.gz johnny@192.168.43.62:/home/johnny/Desktop');
			$install_file = shell_exec('./installFEProd.php');
			return true;
		}
		elseif($data == 'BEpackage-v'){
			$response = $this->deploydb->query($bepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 2;
			$send_file = shell_exec('scp beqav-'.$next.'.tar.gz ethan@192.168.43.8:/home/ethan/Desktop');
			$install_file = shell_exec('./installBEProd.php');
			return true;
		}
		
	}
	
}

public function getPrevious($req) {
	$data = $req['packageName'];
	$fepackage = "Select Max(ver) from fe";
	$bepackage = "Select Max(ver) from be";
		if($data == 'FEpackage-v'){
			$response = $this->deploydb->query($fepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 4;
			$send_file = shell_exec('scp feqav-'.$next.'.tar.gz johnny@192.168.43.62:/home/johnny/Desktop');
			$install_file = shell_exec('./installFPrev.php');
			return true;
		}
		elseif($data == 'BEpackage-v'){
			$response = $this->deploydb->query($bepackage);
			$result = $response->fetch_array(MYSQLI_ASSOC);
			$next = $result['Max(ver)'] - 4;
			$send_file = shell_exec('scp beqav-'.$next.'.tar.gz ethan@192.168.43.8:/home/ethan/Desktop');
			$install_file = shell_exec('./installBPrev.php');
			return true;
		}	
}


}

?>
