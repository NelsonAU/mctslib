from setuptools import setup, find_packages

setup(
	name='mctslib',
	version='0.1',
	author='David Dunleavy',
	author_email='dunleavyd14@gmail.com',
	description='',
	long_description='',
	# tell setuptools to look for any packages under 'src'
	packages=find_packages('src'),
	# tell setuptools that all packages will be under the 'src' directory
	# and nowhere else
	package_dir={'' : 'src'},
	# add an extension module named 'python_cpp_example' to the package 
	# 'python_cpp_example'
	# ext_modules=[CMakeExtension('python_cpp_example/python_cpp_example')],
	# # add custom build_ext command
	# cmdclass=dict(build_ext=CMakeBuild),
	zip_safe=False,
)
