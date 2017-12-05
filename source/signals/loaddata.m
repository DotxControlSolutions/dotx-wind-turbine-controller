%-----------------------------------------------------------------------
%    software    :   DotX Wind Turbine Control Software
%    author      :   DotX Control Solutions BV
%                    1817DC Alkmaar, Netherlands
%                    www.dotxcontrol.com
%                    info@dotxcontrol.com
%-----------------------------------------------------------------------

function D = loaddata( filename )

NAME = loadlist();

DATA = load( filename );
    
for j = 1:size(DATA,2)
    eval( [ 'D.' NAME{j} ' = DATA(:,j) ;'] )
end

%-----------------------------------------------------------------------
