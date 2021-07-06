properties(
	[
		buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')),
		pipelineTriggers([pollSCM('0 H(5-6) * * *')])
	]
)

pipeline
{
	agent any
	options {
		skipDefaultCheckout true
	}
	stages {
		stage( 'Build CMake cross-compilation gcc armhf RaspberryPi') {
			agent{ label "linux/u18.04/gcc-7-armhf" }
			steps {
				checkout scm
				sh '''
					cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/arm-linux-gnueabihf.cmake
					cmake --build build -j10 --target package
				'''
				script {
					def warn = scanForIssues sourceCodeEncoding: 'UTF-8', tool: gcc(id:'gcc compilation warnings RaspberryPI')
					publishIssues failedTotalAll: 1, issues: [warn], name: 'RaspberryPi compilation warnings'	
				}
				dir('build') {
					stash name: 'RaspberryPi-deb', includes: '*.deb'
					stash name: 'RaspberryPi-artifacts', includes: 'eLinkDisplayStatus'
				}
			}			
		}
		
		stage( 'Build CMake cross-compilation gcc armhf NanoPiNeo') {
			agent{ label "linux/u18.04/gcc-7-armhf" }
			steps {
				checkout scm
				sh '''
					cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/arm-linux-gnueabihf.cmake -DPlatformType=NanoPiNeo
					cmake --build build -j10 --target package
				'''
				script {
					def warn = scanForIssues sourceCodeEncoding: 'UTF-8', tool: gcc(id:'gcc compilation warnings NanoPiNeo')
					publishIssues failedTotalAll: 1, issues: [warn], name: 'NanoPiNeo compilation warnings'	
				}
				dir('build') {
					stash name: 'NanoPiNeo-deb', includes: '*.deb'
					stash name: 'NanoPiNeo-artifacts', includes: 'eLinkDisplayStatus'
				}


			}			
		}
		stage( 'Archive') {
			steps {
				dir('RaspberryPi'){ unstash 'RaspberryPi-artifacts' }
				dir('NanoPiNeo'){ unstash 'NanoPiNeo-artifacts' }
				unstash 'RaspberryPi-deb'
				unstash 'NanoPiNeo-deb'
				archiveArtifacts '**'
			}			
		}
	}
	post { 
        changed { 
            emailext body: 'Please go to ${env.BUILD_URL}', to: '${DEFAULT_RECIPIENTS}', subject: "Job ${env.JOB_NAME} (${env.BUILD_NUMBER}) ${currentBuild.currentResult}".replaceAll("%2F", "/")
        }
    }
}
