<?php


/*    @Author: zhuoliang3637@gmain.com    */


class Dao_NssCveInfo{


	private static $tbl_name = 'nss-cve-info';


	public static function retriveNssCveInfoByNssId($nss_id){
	
		$sql = "SELECT * FROM `".self::$tbl_name."` WHERE nss_id='".$nss_id."'";
		$db = new Dao_NssDB();
		return $db->query($sql);
	}
}




?>
