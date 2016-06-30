<?php
/*    @Author: zhuoliang3637@gmail.com    */


include('index.php');

function default_display(){
	$fp = fopen("welcome.html", "r");
	$content = fread($fp, filesize("welcome.html"));
	echo $content;
}

if (isset($_GET['url'])) {
	$url = $_GET['url'];
	$nss_info = Logic_NssInfo::retriveNssInfoByUrl($url);
	if (count($nss_info) == 0){
		default_display();
	}
	else{
		echo '<html><br>';
		echo '<title>View More</title>';
		echo '<head>';
		echo '<link href="css/winterblues.css" rel="stylesheet" type="text/css"/>';
		echo '</head>';
		echo '<body>';
		echo '<div>';
		echo '<table>';
		echo '<caption>NSS INFO</caption>';
		echo '<tr>';
		echo '<th>NSS ID</th>';
		echo '<th>Detection Time</th>';
		echo '<th>Malicious Url</th>';
		echo '<th>Malicious Ip</th>';
		echo '<th>Target Application</th>';
		echo '<th>Victim</th>';
		echo '</tr>';

		foreach ($nss_info as $item) {
			echo '<tr>';
			echo '<td>'.$item['nss_id'].'</td>';
			echo '<td>'.$item['detection_time'].'</td>';
			echo '<td>'.urldecode($item['malicious_url']).'</td>';
			echo '<td>'.$item['malicious_ip'].'</td>';
			echo '<td>'.$item['target_application'].'</td>';
			echo '<td>'.$item['victim'].'</td>';
			echo '</tr>';
		}
		echo '</table>';

		
		echo '<table>';
		echo '<caption>NSS CVE INFO</caption>';
		echo '<tr>';
		echo '<th>NSS ID</th>';
		echo '<th>CVE ID</th>';
		echo '<th>CVE Description</th>';
		echo '</tr>';

		foreach ($nss_info as $item) {
			$nss_cve_info = Logic_NssCveInfo::retriveNssCveInfoByNssId($item['nss_id']);
			foreach ($nss_cve_info as $cve) {
				echo '<tr>';
				echo '<td>'.$cve['nss_id'].'</td>';
				echo '<td>'.$cve['cve_id'].'</td>';
				echo '<td>'.$cve['cve_desc'].'</td>';
				echo '</tr>';
			}	
		}
		echo '</table>';

		echo '<table>';
		echo '<caption>NSS Detected File Information</caption>';
		echo '<tr>';
		echo '<th>NSS ID</th>';
		echo '<th>File Name</th>';
		echo '<th>MD5</th>';
		echo '<th>SHA1</th>';
		echo '<th>SHA256</th>';
		echo '</tr>';

		foreach ($nss_info as $item) {
			$nss_detec_file = Logic_NssDetecFile::retriveNssDetecFileByNssId($item['nss_id']);
			foreach ($nss_detec_file as $detec_file) {
				echo '<tr>';
				echo '<td>'.$detec_file['nss_id'].'</td>';
				echo '<td>'.$detec_file['file_name'].'</td>';
				echo '<td>'.$detec_file['md5_hash'].'</td>';
				echo '<td>'.$detec_file['sha1_hash'].'</td>';
				echo '<td>'.$detec_file['sha256_hash'].'</td>';
				echo '</tr>';
			}
		}
		echo '</table>';
		echo '</div>';
		echo '</body>';
		echo '</html>';
	}
}else if (isset($_GET['hash'])) {
	$hash = $_GET['hash'];
	$nss_detec_file = Logic_NssDetecFile::retriveNssDetecFileByMd5($hash);				
	if (count($nss_detec_file) != 0){
		echo '<html>';
		echo '<title>View More</title>';
		echo '<head>';
		echo '<link href="css/winterblues.css" rel="stylesheet" type="text/css"/>';
		echo '</head>';
		echo '<body>';
		echo '<div>';
		echo '<table>';
		echo '<caption>NSS Detected File Information/caption>';
		echo '<tr>';
		echo '<th>NSS ID</th>';
		echo '<th>File Name</th>';
		echo '<th>MD5</th>';
		echo '<th>SHA1</th>';
		echo '<th>SHA256</th>';
		echo '</tr>';

		foreach ($nss_detec_file as $detec_file) {
			echo '<tr>';
			echo '<td>'.$detec_file['nss_id'].'</td>';
			echo '<td>'.$detec_file['file_name'].'</td>';
			echo '<td>'.$detec_file['md5_hash'].'</td>';
			echo '<td>'.$detec_file['sha1_hash'].'</td>';
			echo '<td>'.$detec_file['sha256_hash'].'</td>';
			echo '</tr>';
		}
		echo '</table>';

		echo '<table>';
		echo '<caption>NSS CVE INFO</caption>';
		echo '<tr>';
		echo '<th>NSS ID</th>';
		echo '<th>CVE ID</th>';
		echo '<th>CVE Description</th>';
		echo '</tr>';

		foreach ($nss_detec_file as $detec_file) {
			$nss_cve_info = Logic_NssCveInfo::retriveNssCveInfoByNssId($detec_file['nss_id']);
			foreach ($nss_cve_info as $cve) {
				echo '<tr>';
				echo '<td>'.$cve['nss_id'].'</td>';
				echo '<td>'.$cve['cve_id'].'</td>';
				echo '<td>'.$cve['cve_desc'].'</td>';
				echo '</tr>';
			}	
		}
		echo '</table>';

		echo '</div>';
		echo '</body>';
		echo '</html>';

	}else {
		default_display();
	}

}elseif (isset($_GET['pe'])) {
	default_display();
}else{
	default_display();
}

?>
