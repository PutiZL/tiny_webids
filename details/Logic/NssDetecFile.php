<?php


/*    @Author: zhuoliang3637@gmain.com    */




class Logic_NssDetecFile{

	public static function retriveNssDetecFileByNssId($nss_id){
		return Dao_NssDetecFile::retriveNssDetecFileByNssId($nss_id);
	}


	public static function retriveNssInfoByMd5($md5){
		$md5 = strtoupper($md5);
		return Dao_NssInfo::retriveNssInfoByMd5($md5);
	}

	public static function retriveNssInfoBySha1($sha1){
		return Dao_NssInfo::retriveNssInfoBySha1($sha1);
	}

	public static function retriveNssInfoBySha256($sha256){
		return Dao_NssInfo::retriveNssInfoBySha256($sha256);
	}
}


?>
