
task :compile do
  sh "ant"
end

WIN32OLE_DIR = '../jruby-win32ole/lib'

task :install do
  cp 'release/x64/racob-x64.dll', WIN32OLE_DIR
  cp 'release/x86/racob-x86.dll', WIN32OLE_DIR
  cp 'release/java/racob.jar', WIN32OLE_DIR
end

task :default => [:compile, :install]
