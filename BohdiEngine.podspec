#
# Be sure to run `pod lib lint BohdiEngine.podspec' to ensure this is a
# valid spec before submitting.
#

Pod::Spec.new do |s|
  s.name             = 'BohdiEngine'
  s.version          = '1.1.2'
  s.summary          = 'A simple 3D engine written by Monk-C'
  s.description      = 'BohdiEngine is a simple 3D model rendering engine. it is cross platform'

  s.homepage         = 'https://github.com/sunpaq/BohdiEngine-pod'
  s.license          = { :type => 'BSD', :file => 'LICENSE' }
  s.author           = { 'Sun YuLi' => 'sunpaq@gmail.com' }
  s.source           = { :git => 'https://github.com/sunpaq/BohdiEngine-pod.git', :tag => s.version.to_s }

  s.ios.deployment_target = '8.0'

  s.source_files         = 'BohdiEngine/**/**/*'
  s.public_header_files  = 'BohdiEngine/Classes/**/*.h'
  s.private_header_files = 'BohdiEngine/External/**/*.h'

end
