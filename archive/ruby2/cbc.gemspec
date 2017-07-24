lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

Gem::Specification.new do |spec|
  spec.name          = "cbc"
  spec.version       = '0.0.1'
  spec.authors       = ["Craig Bowles"]
  spec.email         = ["staticimport@gmail.com"]
  spec.summary       = %q{Cranberry Transpiler to C}
  spec.description   = %q{Custom language known as 'cranberry' being transcompiled to efficient, portable C}
  spec.homepage      = "https://github.com/staticimport/cranberry"
  spec.license       = "MIT"

  spec.files         = ['lib/cbc.rb']
  spec.executables   = ['bin/cbc'] # TODO: add ibc
  spec.test_files    = ['tests/test_foo.rb']
  spec.require_paths = ["lib"]
end
