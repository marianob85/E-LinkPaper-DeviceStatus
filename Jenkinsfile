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
		stage( 'Build autoconf gcc') {
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
				'''
		
				cleanWs()
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
					cp ./eLinkDisplayStatus ./eLinkDisplayStatus-arm-linux-gnueabihf-gcc
				'''
				archiveArtifacts artifacts: 'CMake/eLinkDisplayStatus-arm-linux-gnueabihf-gcc', onlyIfSuccessful: true
				cleanWs()
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
				
				warnings canComputeNew: false, canResolveRelativePaths: false, categoriesPattern: '', consoleParsers: [[parserName: 'GNU Make + GNU C Compiler (gcc)'], [parserName: 'Clang (LLVM based)']], defaultEncoding: '', excludePattern: '', failedTotalAll: '1', failedTotalHigh: '1', failedTotalLow: '1', failedTotalNormal: '1', healthy: '', includePattern: '', messagesPattern: '', unHealthy: ''
			}			
		}
	}
}
