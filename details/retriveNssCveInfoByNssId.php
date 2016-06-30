<?php


/*    @Author: zhuoliang3637@gmain.com    */


include('index.php');

$result = array();

if (isset($_GET['nssid'])) {
	$nss_id = $_GET['nssid'];
	$result['status'] = 200;
	$result['result'] = Logic_NssCveInfo::retriveNssCveInfoByNssId($nss_id);
}else {
	$result['status'] = 200;
	$result['result'] = 'Please check your parameters!';
}


echo json_encode($result);

?>
