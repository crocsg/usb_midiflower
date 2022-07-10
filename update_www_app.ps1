Remove-Item './www/fs/*' -Recurse
Copy-Item -Filter *.* -Path './reactconfig/flower/build/*' -Recurse -Destination './www/fs/'
Copy-Item  -Path './reactconfig/flower/build/index.html' -Recurse -Destination './www/fs/404.html'