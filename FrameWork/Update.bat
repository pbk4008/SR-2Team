xcopy /y		.\Engine\Headers\*.*			.\Reference\Headers\
xcopy /y		.\Engine\Export\*.*				.\Reference\Headers\

xcopy /y		.\Engine\System\Code\*.h		.\Reference\Headers\
xcopy /y		.\Engine\Utility\Code\*.h		.\Reference\Headers\

xcopy /y		.\Engine\System\Bin\System.dll	.\Client\Bin\
xcopy /y		.\Engine\System\Bin\System.lib	.\Reference\Library\
xcopy /y		.\Engine\Utility\Bin\Utility.dll	.\Client\Bin\
xcopy /y		.\Engine\Utility\Bin\Utility.lib	.\Reference\Library\