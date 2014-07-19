((nil . ((eval . (setq project-root (locate-dominating-file
									 buffer-file-name ".dir-locals.el")
					   ))))
 (c++-mode . ((compile-command . (format "cd %s../build/debug && qmake ../../SheepBattleBoats/SheepBattleBoats.pro -r -spec linux-g++-64 && make -j4" project-root))))
 (c-mode . ((compile-command . (format "cd %s../build/debug && qmake ../../SheepBattleBoats/SheepBattleBoats.pro -r -spec linux-g++-64 && make -j4" project-root)))))
