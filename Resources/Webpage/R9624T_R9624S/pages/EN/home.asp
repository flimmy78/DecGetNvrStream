<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<meta http-equiv="Pragma" content="no-cache">
<title>DVR Remote Management System</title>
<style type="text/css">
<!--
#Layer1 {
	position:absolute;
	width:200px;
	height:115px;
	z-index:1;
	top: 44px;
}
.STYLE7 {font-size: 14px; color:#FFFFFF}
.STYLE8 {
font:"����";
font-size: 16px; }
.down {color:yellow; background: navy; font-style: italic}
.over {color:yellow; background: navy}
.style18 {font-family: "��Բ"; font-size: 18px; font-weight: bold; color:E0E0E0; }
.style19 {font-size: 18px}
.style20 {font-family: "��Բ"; font-size: 12px; color:#000000}
.style21 {font-family: "��Բ"; font-size: 18px; font-weight: bold;  color:E0E0E0;}
-->
</style>
<style type="text/css" media="all" title="Default">
      @import "/css/main.css";
      @import "/css/slider1.css";
.style25 {
	font-size: 16px;
	font-weight: bold;
}
.style26 {font-size: 16px; font-family: "����_GB2312";}
</style>
<script language="JavaScript" type="text/JavaScript">
<!--
function MM_reloadPage(init) {  //reloads the window if Nav4 resized
  if (init==true) with (navigator) {if ((appName=="Netscape")&&(parseInt(appVersion)==4)) {
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();
}
MM_reloadPage(true);
//-->
</script>

<script language="JavaScript" type="text/JavaScript">
function SubmitKeyClick()
{
		if (event.keyCode   ==   13)     
    {
        document.getElementById("aa").focus();              
     }   

}
</script>

</head>
<body onunload="stop_all()" onselectstart="return false;" ondragstart="return false" onload="initasp()">
<script language="javascript" type="text/javascript" src="/js/web_homecache.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/web_preview.js"></script>
<div id="login_layer" name="login_layer" style="z-index:0;display:block;">
<form onsubmit="return do_login()">
	<input type="hidden" name="enflag" id="enflag" value="1"/>
  <TABLE cellSpacing=0 cellPadding=0  bgcolor="#5A5A5C" width="1000" height="768"  align=center background="/images/logback.jpg">
  
  
   <tr>
   <td width="874" height="102">&nbsp;</td>
   
   
   <td width="126" valign="bottom">  
   <span style="border:0px solid #000000; width:82px; height:16px">	  
	<SELECT  name="Language" style="width:140px; clip:rect(2 140 16 2);cursor:hand;" onchange="ChangLanguage(this.value)";" onkeydown="SubmitKeyClick()" ><OPTION value=0 >Simplified Chinese</OPTION><OPTION value=1 selected>English</OPTION><OPTION value=2 >Traditional Chinese</OPTION></SELECT></span>
   </td>   
   </tr>
   
   <tr>
   <td align="2" height="186">
   </td>
   </tr>
   
   <tr>
   <td colspan="2" height="220" >
   
   <TABLE cellpadding="0">
   
     <tr>
   <td width="590">&nbsp;</td>
   <td width="280" height="20" valign="bottom" colspan="2">&nbsp;
   </td>	
   </tr>
   
  <tr>
  <td width="590">&nbsp;</td>
  <td align="left" valign="middle" width="242"  height="24" class="STYLE3">
	<span class="STYLE3" style="color:#A8A8A8;">&nbsp;&nbsp;User Name:&nbsp;<input type="text" height="30px"  name="username" id="username" value="admin" style="width:158px; height:18px; border:0px solid #000000;" onkeydown="SubmitKeyClick()" onFocus="this.select()" onMouseOver="this.focus()" onClick="if (this.value=='admin') this.value=''" onBlur="if (this.value =='') this.value='admin'">
	</span>
  </td>
  </tr>
   
   
   <tr>
   <td width="590" height="40" >&nbsp;</td>
   <td   align="left" width="242" height="24" class="STYLE3" style="color:#A8A8A8;" valign="middle">&nbsp;&nbsp;Password:&nbsp;&nbsp;&nbsp;
	  <input height="10" type="password" name="loginpass" id="loginpass"   style="border:0px solid #000000; width:158px;" maxlength="6" AUTOCOMPLETE="off" onkeydown="SubmitKeyClick()">
   </td>
   </tr>
   

   
   
   <tr>
   <td width="590">&nbsp;</td>
   <td align="left" height="24" width="242" >
      <span style="border:0px solid #000000; width:158px; color:#A8A8A8; height:16px">&nbsp;&nbsp;Network: &nbsp;&nbsp;	&nbsp;&nbsp;&nbsp
	<SELECT id="nettype" name="nettype" style="width:158px; clip:rect(2 158 16 2);" onkeydown="SubmitKeyClick()" ><OPTION value=0>LAN</OPTION><OPTION value=1 selected>WAN</OPTION></SELECT></span>
   </td>
   
   
   <td height="24" width="62" align="center" onclick="do_login()" background="../images/log.png" onmouseover="this.background='../images/log-1.png'" onmouseout="this.background='../images/log.png'" colspan=1 align=center style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:��Բ "></td>
   
 
   
   </tr>
   
   <tr>
   <td colspan="2" height="20" >&nbsp;</td>
   </tr>
   
   
   <tr>
   <td width="590">&nbsp;</td>
   <td width="280" height="60" valign="bottom" colspan="2">
	<span style="color:#A8A8A8;font-size:12px">&nbsp;&nbsp;If plugin cann't be installed automatically, pls &nbsp;&nbsp;download<a 
href="../ActiveXSetup.zip" style="color:#A8A8A8"  target=_blank>manual installation package</a></span>
   </td>	
   </tr>
   
   
   
   </TABLE>
   
   </td>
   
   </tr>
   
   
   
   
   <tr>
   
   <td colspan="3" height="260">&nbsp; 
   </td>
   
   </tr>
   
</TABLE>

</form>
</div>




<div id="preview_layer" name="preview_layer" style="z-index:1;display:none;">


<TABLE  height="571" width="937" style="border-bottom:6px #000000 solid" cellSpacing=0 cellPadding=0   align=center >

<tr>
<td colspan="2">
<table cellSpacing=0 cellPadding=0 width="937" background="/images/top.png">

<tr>
      <td height="24" width="784"  colspan=1 align=left valign="bottom" style="color:#E0E0E0; font-size:18px; font-family:��Բ " >&nbsp;&nbsp;&nbsp;H.264 Network Video Surveillance System</td>
	  <td width="148" colspan=1 >
		<TABLE cellSpacing=0 cellPadding=0 ><tr>
			<td width="121" align="center" >
				<div  id="user" style="color:#E0E0E0; font-size:14px; font-family:��Բ " align="right" ></div>			
			</td>
		
	
			
		<!--
			<td align="right" bgcolor="#990000">
			
			<input type="button" type="button" value="ע��" style="width:70px; background-image:url(/images/logout.png); font-size:14px; height:24px; border:0; color:#E0E0E0 " onmouseover="this.style.background-image='url(/images/logout-1.png)'"  onmouseout="this.style.background-image:url(/images/logout.png)" onclick="location.reload()" />
				
			
			</td>
			
			-->
			
		<td height="24" onclick="location.reload()" onmouseover="this.background='../images/logout-1.png'" onmouseout="this.background='../images/logout.png'" width="72"  colspan=1 align=center valign="middle" style="cursor:hand;color:#E0E0E0; font-size:16px; font-family:��Բ ">Logout</td>
			
			
			
			
		</tr></TABLE >	</td>	
</tr>
	
	  
<tr>
	<td height="21" colspan=2 align=center  class="style18">&nbsp;</td>
</tr>
</table></td>
</tr>




  <TR>
    <TD width=714 valign="top" >
      <TABLE style="border-left:4px #000000 solid " cellSpacing=0 cellPadding=0 width=709 height="571" border=0 >
	  
	 
		  
		 <tr>
          <TD height=571 valign="middle" align="right" ><object codebase="../TLNetDvr.CAB#version=2,1,6,5" id="TLNetDvrCtrl" classid="CLSID:6714928B-F4BF-4E44-82EF-BB036DBD9213" height=571 width=709 align="top">
            <embed width="709" height="571" align="top"></embed>
          </object>          </TD>
         </TR>
		  </TABLE>	</TD>
	
		  
    <TD width="310" valign="top" height="571" align="left"><table cellspacing=0 style="border-right:4px #000000 solid; visibility:visible "  cellpadding=0 height="571" width=224 bgcolor="5A5A5C" >
      <tr>
        <td colspan=6 height="11"></td>
      </tr>
      <tr> </tr>
      <tr>
        <td width="37" >&nbsp;</td>
        <td  width="52" align=center ><img src="/images/up.png" style="cursor:hand;" width="24" height="24" onmousemove="mouseover(this,2)" onmouseout="mouseout(this,2)" onmousedown="ptz_move_up()" onmouseup="ptz_stop()"/> </td>
        <td width="36">&nbsp;</td>
        <td align="right" ><img src="/images/small.png" style="cursor:hand;" alt="IRIS-" width="24" height="24" onmousemove="mouseover(this,5)" onmouseout="mouseout(this,5)" onmousedown="ptz_aper_decrease()" onmouseup="ptz_stop()"/></td>
        <td  class="style20"  align="center" >IRIC</td>
        <td  width="36" align="left" ><img src="/images/big.png" style="cursor:hand;" alt="IRIS+" width="24" height="24" onmousemove="mouseover(this,6)" onmouseout="mouseout(this,6)" onmousedown="ptz_aper_increase()" onmouseup="ptz_stop()"/></td>
      </tr>
      <tr>
        <td align="right"  height="38" ><img src="/images/left.png" style="cursor:hand;" width="24" height="24" onmousemove="mouseover(this,1)" onmouseout="mouseout(this,1)" onmousedown="ptz_move_left()" onmouseup="ptz_stop()" /> </td>
        <td align="center" ><img src="/images/ptz.png" style="cursor:hand;" onmousemove="mouseover(this,11)" onmouseout="mouseout(this,11)" width="24" height="24"/></td>
        <td  align="left"><img src="/images/right.png" style="cursor:hand;" onmousemove="mouseover(this,3)" onmouseout="mouseout(this,3)" width="24" height="24" onmousedown="ptz_move_right()" onmouseup="ptz_stop()"/> </td>
        <td align="right"><img src="/images/small.png" style="cursor:hand;" alt="Focus-" width="24" height="24" onmousemove="mouseover(this,5)" onmouseout="mouseout(this,5)" onmousedown="ptz_focus_nearer()" onmouseup="ptz_stop()"/></td>
        <td  class="style20" align="center">&nbspFocus&nbsp</td>
        <td align="left"><img src="/images/big.png" style="cursor:hand;" alt="Focus+" width="24" height="24" onmousemove="mouseover(this,6)" onmouseout="mouseout(this,6)" onmousedown="ptz_focus_farther()" onmouseup="ptz_stop()"/></td>
      </tr>
      <tr>
        <td>&nbsp;</td>
        <td  align="center" ><img src="/images/down.png" style="cursor:hand;" width="24" height="24" onmousemove="mouseover(this,4)" onmouseout="mouseout(this,4)" onmousedown="ptz_move_down()" onmouseup="ptz_stop()"/> </td>
        <td>&nbsp;</td>
        <td align="right" ><img src="/images/small.png" style="cursor:hand;" alt="Zoom-" width="24" height="24" onmousemove="mouseover(this,5)" onmouseout="mouseout(this,5)" onmousedown="ptz_zoom_out()" onmouseup="ptz_stop()"/></td>
        <td class="style20" align="center">&nbspZoom&nbsp</td>
        <td align="left" ><img src="/images/big.png" style="cursor:hand;" alt="Zoom+" width="24" height="24" onmousemove="mouseover(this,6)" onmouseout="mouseout(this,6)" onmousedown="ptz_zoom_in()" onmouseup="ptz_stop()"/></td>
      </tr>
      <tr>
        <td colspan=6 height="11"></td>
      </tr>
      <tr>
        <td colspan="6" align=center><table align=center border=0>
            <tr >
              <td style="padding-left:5px"; width=55><img src="/images/one.png" style="cursor:hand;"   width="32" height="32" onmousemove="mouseover(this,7)" onmouseout="mouseout(this,7)" onclick="do_set_display_wndnum(1)"/> </td>
              <td  width=55 ><img src="/images/four.png" width="32" height="32" style="cursor:hand;" onmousemove="mouseover(this,8)" onmouseout="mouseout(this,8)" onclick="do_set_display_wndnum(4)"/> </td>
							<td  width=55 ><img style="cursor:hand;" src="/images/nine.png" width="32" height="32" onmousemove="mouseover(this,9)" onmouseout="mouseout(this,9)" onclick="do_set_display_wndnum(9)"/> </td>
              <td  width=55 ><img style="cursor:hand;" src="/images/sixteen.png" width="32" height="32" onmousemove="mouseover(this,10)" onmouseout="mouseout(this,10)" onclick="do_set_display_wndnum(16)"/> </td>
              <td  width=55 ><img style="cursor:hand;" src="/images/25scr.png" width="32" height="32" onmousemove="mouseover(this,25)" onmouseout="mouseout(this,25)" onclick="do_set_display_wndnum(25)"/> </td>
            </tr>
        </table></td>
      </tr>
      <tr>
        <td colspan=6 align=center><table cellspacing=0  border=0 >
		<tr><td>&nbsp;</td></tr>
                <tr>
				<td>
				<TABLE align=center  cellpadding=0 cellSpacing=0 border=0 >
            <tr>
				<td colspan="6" align="center"><span style="padding-left:5px; background-color:#5A5A5C">
				  <select id="streamtype" name="streamtype" style="background:#5A5A5C; clip:rect(2 202 20 2);margin:-2;width:206px;"  onchange="stop_all()">
                  <option  value=0 selected>Main Stream</option>
                  <option value=1 >Sub Stream</option>
                </select>
				</span></td>
				</tr>
				<!--<tr colspan="6"><td>&nbsp;</td></tr>-->
				<tr align="center" width=80>
				<td width=10>&nbsp;</td>
				<td width=104 style="cursor:hand;padding-left:5px;color:#E0E0E0; font-size:14px; font-family:�׈A "  height=20 align="center" onclick="start_all()" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'">Start All</td>
				<td width=109 style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:�׈A " height=20 align="center" onclick="stop_all()" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'">Stop All</td>
				<td width=6>&nbsp;</td>
				</tr>
				</TABLE>
            </td>
            </tr>
            
            <tr>
              <td  align="left" onclick="tableShow()" ><img align="left" style="cursor:hand;" src="/images/00.png" width=50 height="16" border=0 /></td>
            </tr>
            <tr>
              <td colspan=6 valign=top><table id="bbb" valign=top style="visibility:visible">
			  <tr>
                          <td align=right width="30"><img id=pic0  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td width="38" onclick="control_channel(0)" id="ch0" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;1</td>
                          <td width="15"  align="left"><img src="/images/video.png" name="recpic0" id="recpic0" width="10" height="10" /></td>
                          
						  <td align=right width="30"><img id=pic1  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(1)" id="ch1" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;2</td>
                          <td width="10"><img src="/images/video.png" name="recpic1" id="recpic1" width="10" height="10" /></td>
                        </tr>
                         <tr>
                          <td align=right><img id=pic2  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(2)" id="ch2" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;3</td>
                          <td align="left"><img src="/images/video.png" name="recpic2" id="recpic2" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic3  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(3)" id="ch3" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;4</td>
                          <td><img src="/images/video.png" name="recpic3" id="recpic3" width="10" height="10" /></td>
                        </tr>
						 <tr>
                          <td align=right><img id=pic4  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(4)" id="ch4" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;5</td>
                          <td align="left"><img src="/images/video.png" name="recpic4" id="recpic4" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic5  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(5)" id="ch5" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;6</td>
                          <td><img src="/images/video.png" name="recpic5" id="recpic5" width="10" height="10" /></td>
                        </tr> <tr>
                          <td align=right><img id=pic6  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(6)" id="ch6" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;7</td>
                          <td align="left"><img src="/images/video.png" name="recpic6" id="recpic6" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic7  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(7)" id="ch7" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;8</td>
                          <td><img src="/images/video.png" name="recpic7" id="recpic7" width="10" height="10" /></td>
                        </tr> 
						<tr>
                          <td align=right><img id=pic8  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(8)" id="ch8" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;9</td>
                          <td align="left"><img src="/images/video.png" name="recpic8" id="recpic8" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic9  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(9)" id="ch9" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;10</td>
                          <td><img src="/images/video.png" name="recpic9" id="recpic9" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic10  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(10)" id="ch10" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;11</td>
                          <td align="left"><img src="/images/video.png" name="recpic10" id="recpic10" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic11  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(11)" id="ch11" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;12</td>
                          <td><img src="/images/video.png" name="recpic11" id="recpic11" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic12  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(12)" id="ch12" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;13</td>
                          <td align="left"><img src="/images/video.png" name="recpic12" id="recpic12" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic13  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(13)" id="ch13" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;14</td>
                          <td><img src="/images/video.png" name="recpic13" id="recpic13" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic14  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(14)" id="ch14" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;15</td>
                          <td align="left"><img src="/images/video.png" name="recpic14" id="recpic14" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic15  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(15)" id="ch15" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;16</td>
                          <td><img src="/images/video.png" name="recpic15" id="recpic15" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic16  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(16)" id="ch16" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;17</td>
                          <td align="left"><img src="/images/video.png" name="recpic16" id="recpic16" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic17  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(17)" id="ch17" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;18</td>
                          <td><img src="/images/video.png" name="recpic17" id="recpic17" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic18  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(18)" id="ch18" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;19</td>
                          <td align="left"><img src="/images/video.png" name="recpic18" id="recpic18" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic19  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(19)" id="ch19" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;20</td>
                          <td><img src="/images/video.png" name="recpic19" id="recpic19" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic20  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(20)" id="ch20" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;21</td>
                          <td align="left"><img src="/images/video.png" name="recpic20" id="recpic20" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic21  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(21)" id="ch21" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;22</td>
                          <td><img src="/images/video.png" name="recpic21" id="recpic21" width="10" height="10" /></td>
                        </tr>
						<tr>
                          <td align=right><img id=pic22  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(22)" id="ch22" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;23</td>
                          <td align="left"><img src="/images/video.png" name="recpic22" id="recpic22" width="10" height="10" /></td>
                          
						  <td align=right><img id=pic23  src="/images/ch.png" width="26" height="16" border=0 /></td>
                          <td onclick="control_channel(23)" id="ch23" class="style20" style="CURSOR: hand;padding-left:10px;" align=left>CH &nbsp;24</td>
                          <td><img src="/images/video.png" name="recpic23" id="recpic23" width="10" height="10" /></td>
                        </tr>
                  <tr>
                    <td colspan=6>&nbsp;</td>
                  </tr>
              </table></td>
            </tr>
        </table></td>
      </tr>
    </table></TD>
  </TR>
			
		 <tr valign="top">
		 <td colspan="2">
		 <table cellSpacing=0 cellPadding=0 height="36" background="/images/bottom.png" style="border-left:4px #000000 solid; border-right:4px #000000 solid" width="937">
		 <td height="36" width="64" align="center"  onclick="window.open('../DVRPlayerSetup.rar')" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'" colspan=1 style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:�׈A ">������</td>
	  	
			  <td width="630"></td>
			  
			<!--  <TD height="36" width="72" align="center"><INPUT type="button" value="Start VOIP" onclick="do_voip(this)">&nbsp; -->
			  <td height="36" width="87" align="center"  onclick="do_voip(this)" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'" colspan=1 style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:��Բ ">Start VOIP</td>
			  
			    <td height="36" width="63" align="center"  onclick="do_configure()" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'" colspan=1  style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:��Բ ">Configure</td>
				
			   <td height="36" width="108" align="center"  onclick="do_playback()" onmouseover="this.background='../images/playback-1.png'" onmouseout="this.background='../images/playback.png'" colspan=1  style="cursor:hand;color:#E0E0E0; font-size:14px; font-family:��Բ ">Playback</td>
			
		   </table>			</tr>
			</TABLE>
</div>

</body>
</html>

