<?php

class Logic_NssInfo{

	public static function retriveNssInfoByUrl($url){
		$url = urlencode($url);
		$url = str_replace('%2F', '/', $url);
		return Dao_NssInfo::retriveNssInfoByUrl($url);
	}

}

?>
