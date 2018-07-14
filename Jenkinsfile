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
		stage( 'Build autoconf cross-compilatio gcc armhf') {
			agent {
				node {
					label 'linux && development'
				}
			}
			steps {
				sh '''
					cd autoconf
					autoreconf --install --force
					./configure
					make -j4
					cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-gcc-autoconf
				'''
				archiveArtifacts artifacts: 'autoconf/eLinkDisplayStatus-arm-linux-gnueabihf-gcc-autoconf', onlyIfSuccessful: true
				cleanWs()
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'GNU Make + GNU C Compiler (gcc)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '0', healthy: '', includePattern: '', messagesPattern: '', unHealthy: '', unstableTotalAll: '0'
			}			
		}

		stage( 'Build CMake cross-compilation gcc armhf') {
			agent {
				node {
					label 'linux && development'
				}
			}
			steps {
				sh '''
					cd CMake
					cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-gnueabihf.cmake -Wno-dev 
					make -j4
					make package
					cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-gcc
				'''
				archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnueabihf-gcc', onlyIfSuccessful: true
				archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus*.deb', onlyIfSuccessful: true
				cleanWs()
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'GNU Make + GNU C Compiler (gcc)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '0', healthy: '', includePattern: '', messagesPattern: '', unHealthy: '', unstableTotalAll: '0'

			}			
		}
		stage( 'Build CMake cross-compilation llvm armhf') {
			agent {
				node {
					label 'linux && development'
				}
			}
			steps {
				sh '''
					cd CMake
					cmake -DCMAKE_TOOLCHAIN_FILE=arm-linux-llvm.cmake -Wno-dev 
					make -j4
					cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-llvm
				'''
				archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnueabihf-llvm', onlyIfSuccessful: true
				cleanWs()
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'Clang (LLVM based)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '0', healthy: '', includePattern: '', messagesPattern: '', unHealthy: '', unstableTotalAll: '0'
			}			
		}
	}
}
