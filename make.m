% Automated .c and .cpp into .mex file compilation.
% Traversing the directory hierarchy, this function compiles any .c or .cpp
% source files it finds into .mex (MatLab executable) functions if their
% modification date is later than the compiled version.

% Copyright 2008-2009 Levente Hunyadi
function make(mode, varargin)

%addpath(pwd, fullfile(pwd,'bin'));

fprintf( ...
    [ 'Automated .mex compiler\n', ...
      'Copyright 2008-2009 Levente Hunyadi\n', ...
      '\n' ...
    ]);

if nargin >= 1
    switch mode
        case 'clean'
            args = varargin;
            mode = 'clean';
        case 'build'
            args = varargin;
            mode = 'build';
        otherwise
            args = [{mode}, varargin];
            mode = 'make';
    end
else
    mode = 'make';
    args = {};
end

fprintf('Compiling source files...\n');

% if exist('lib', 'dir')
%     deflibpath = fullfile(matlabroot, 'extern', 'lib', computer('arch'), 'microsoft');  % default library path
%     addlibpath = fullfile(cd, 'lib', computer('arch'));  % additional library path
% 
%     libraryswitch = sprintf('-L%s', addlibpath);
%     linkswitch = sprintf('LINK_LIB=%s', [deflibpath ';' addlibpath]);
%     setenv('LIB', [deflibpath ';' addlibpath]);
% 
%     args = [{libraryswitch}, {linkswitch}, args];
%     fprintf('"lib" library directory added.\n');
% end

% add every found library to compile string 
if exist('lib', 'dir')
   items = dir('lib');
   for i=1:length(items)
       itemfull = fullfile(cd, 'lib', items(i).name);
       [~, ~, fileext] = fileparts(itemfull);
       if ~items(i).isdir && strcmp(fileext,'.a')
           args = [{itemfull}, args];
       end
   end
end

if exist('inc', 'dir')
    includeswitch = sprintf('-I%s', fullfile(cd, 'inc'));
    args = [{includeswitch}, args];
    fprintf('"inc" source header directory added.\n');
end

% error checking
if isempty(cell2mat(strfind(args,'mexutil')))
	error('the mexutil library is required'); 
end
if ~exist('bin', 'dir')
    error('need bin directory');
end

make_walker(cd, cd, mode, args);

% Traverses a directory structure compiling files in each directory if needed.
function make_walker(dirpath, rootpath, mode, args)

% put mex function in same dir or root bin dir?
if (1)
    binpath  = fullfile(rootpath,'bin');
else
    binpath  = dirpath;
end
binitems = dir(binpath);

items = dir(dirpath);
for i = 1 : length(items)
    item = items(i);
    itemfull = fullfile(dirpath, item.name);
    [~, filename, fileext] = fileparts(itemfull); 
    
    if item.isdir && ~any(strcmp(item.name, {'.','..','inc','lib','doc','cfg'}))
        % look for MEX source files in subdirectory
        make_walker(itemfull, rootpath, mode, args);  
    elseif ismexsource(itemfull)
        mexfileext = ['.', mexext];
        itemmex = [];
        for j=1:length(binitems)
            if strcmp([filename, mexfileext], binitems(j).name)
               itemmex = binitems(j); 
            end
        end
        switch mode
            case 'build'
                make_mex(itemfull, binpath, args);
            case 'make'
                if isempty(itemmex) || itemmex.datenum < item.datenum 
                    % compiled file does not exist, or not up-to-date, or user requested full build
                    make_mex(itemfull, binpath, args);
                else
                    fprintf('%s%s [up-to-date]\n', filename, fileext);
                end
            case 'clean'
                if ~isempty(itemmex)
                    delete(fullfile(binpath, [filename mexfileext]));
                    fprintf('%s%s [deleted]\n', filename, mexfileext);
                    if exist(fullfile(binpath, [filename '.m']),'file')
                        delete(fullfile(binpath, [filename '.m']));
                    end
                end
        end
    end
end

function make_mex(itemfull, binpath, args)

[filepath, filename, fileext] = fileparts(itemfull); 

if exist(fullfile(filepath, [filename '.m']), 'file')
    copyfile(fullfile(filepath, [filename '.m']), fullfile(binpath, [filename '.m']));
end

try
    mex('-largeArrayDims', args{:}, '-outdir', binpath, itemfull);
    fprintf('%s%s [ok]\n', filename, fileext);
catch me  %#ok<NASGU>
    fprintf('%s%s [error]\n', filename, fileext);
end

function rv = ismexsource(itemfull)

[~, ~, fileext] = fileparts(itemfull);

rv = 0;
if (strcmp(fileext,'.c') || strcmp(fileext,'.cpp'))
    % read the file
    filetext = fileread(itemfull);
    % search for the line of code that includes 'mexFunction'
    % each line is separated by a newline ('\n')
    expr = '[^\n]*mexFunction[^\n]*';
    fileread_info = regexp(filetext, expr, 'match');

    if ~isempty(fileread_info)
        rv = 1;
    end
end

%% license.txt
% Copyright (c) 2009, Levente Hunyadi
% All rights reserved.
% 
% Redistribution and use in source and binary forms, with or without 
% modification, are permitted provided that the following conditions are 
% met:
% 
%     * Redistributions of source code must retain the above copyright 
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright 
%       notice, this list of conditions and the following disclaimer in 
%       the documentation and/or other materials provided with the distribution
%       
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
% POSSIBILITY OF SUCH DAMAGE.
