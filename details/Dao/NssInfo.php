<?php

class Dao_NssInfo{

	private static $tbl_name = 'nss-exploit';
	
	public static function retriveNssInfoByUrl($url){
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE malicious_url='".$url."'";
		$db = new Dao_NssDB();
		return $db->query($sql);	
	}

}
?>
