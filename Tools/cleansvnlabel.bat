@ECHO OFF
@echo ===================================================================
@echo    ���svn,vss,cvs����ļ�   author:Yusheng date:2009.4.24
@echo    filename : clearSvnCvsVss.bat
@echo ===================================================================
@ECHO ��Ctrl + Cȡ����
@pause
@echo ִ��������%0 %1 %2 %3
@echo ת����%1 
cd /d %1

@echo (1)��ʼ�����.SVN���ļ���
@rem for /r %%d in (.) do if exist "%%d\.svn" echo /s /q "%%d\.svn"
@for /r %%d in (.) do if exist "%%d\.svn" rd /s /q "%%d\.svn"

