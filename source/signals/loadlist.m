%-----------------------------------------------------------------------
%    software    :   DotX Wind Turbine Control Software
%    author      :   DotX Control Solutions BV
%                    1817DC Alkmaar, Netherlands
%                    www.dotxcontrol.com
%                    info@dotxcontrol.com
%-----------------------------------------------------------------------

function NAME = loadlist()

fid = fopen( 'signal_definitions_external.h', 'r' );

tag = '  ';
NAME = {};
N = 0;

while ~strcmp( tag, 'enum' )
    s = fgetl( fid );
    tag = sscanf( s, '%s LOGSIGNALS{' );
end
while 1     
    s = fgetl( fid );
    if s < 0, break, end
    tag = sscanf( s, ' %s ,' );
    if strcmp( tag, 'MAXLOG' ), break, end
    if ~strcmp( tag, '') 
        N = N+1;
        NAME{N} = tag;
    end    
end
fclose(fid);    

%-----------------------------------------------------------------------