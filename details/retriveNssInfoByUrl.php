<?php

include('index.php');

$result = array();

if (isset($_GET['url'])) {
	$url = $_GET['url'];
	$result['status'] = 200;
	$result['result'] = Logic_NssInfo::retriveNssInfoByUrl($url);
}else {
	$result['status'] = 201;
	$result['result'] = 'Please check your parameters!';
}


echo json_encode($result);
?>
