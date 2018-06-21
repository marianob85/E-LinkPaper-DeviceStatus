properties(
	[
		buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')),
		pipelineTriggers([pollSCM('0 H(5-6) * * *')])
	]
)

pipeline
{
	agent { node { label 'linuxarmhf && development' } }
	stages
	{
		stage('Build autoconf'){
			steps {
				sh '''
					cp -R src autoconf/src/
					cd autoconf
					autoreconf --install --force
					./configure
					make
				'''

			}
		}
		stage('Package'){
			steps {
				sh '''
				'''
			}
		}
		
		stage('Archive'){
			steps {
				archiveArtifacts artifacts: 'src/eLinkDisplayStatus', onlyIfSuccessful: true
			}
		}
		
		stage('CleanUp'){
			steps {
				deleteDir()
			}
		}
	}
}
