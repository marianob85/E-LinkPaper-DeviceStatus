properties(
	[
		buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')),
		pipelineTriggers([pollSCM('0 H(5-6) * * *')])
	]
)

pipeline
{
	agent none
	stages {
	
		stage( 'Build CMake cross-compilation gcc armhf RaspberryPi') {
			agent {
				node {
					label 'linux && stretch'
				}
			}
			steps {
				sh '''
					cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=CMake/toolchain/arm-linux-gnueabihf.cmake
					cmake --build build -j10 --target package
				'''
				dir('build') {
					stash name: 'RaspberryPi-deb', includes: '*.deb'
					stash name: 'RaspberryPi-artifacts', includes: 'eLinkDisplayStatus'
				}
				
				cleanWs()
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'GNU Make + GNU C Compiler (gcc)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '0', healthy: '', includePattern: '', messagesPattern: '', unHealthy: '', unstableTotalAll: '0'

			}			
		}
		
		stage( 'Build CMake cross-compilation gcc armhf NanoPiNeo') {
			agent {
				node {
					label 'linux && stretch'
				}
			}
			steps {
				sh '''
					cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=CMake/toolchain/arm-linux-gnueabihf.cmake -DPlatformType=NanoPiNeo
					cmake --build build -j10 --target package
				'''
				dir('build') {
					stash name: 'NanoPiNeo-deb', includes: '*.deb'
					stash name: 'NanoPiNeo-artifacts', includes: 'eLinkDisplayStatus'
				}
				cleanWs()
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'GNU Make + GNU C Compiler (gcc)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '0', healthy: '', includePattern: '', messagesPattern: '', unHealthy: '', unstableTotalAll: '0'

			}			
		}
		stage( 'Archive') {
			agent {
				node {
					label 'master'
				}
			}
			steps {
			
				dir('artifacts') {
					dir('RaspberryPi'){ unstash 'RaspberryPi-artifacts' }
					dir('NanoPiNeo'){ unstash 'NanoPiNeo-artifacts' }
					unstash 'RaspberryPi-deb'
					unstash 'NanoPiNeo-deb'
					archiveArtifacts '**'
				}
			}			
		}
	}
}
