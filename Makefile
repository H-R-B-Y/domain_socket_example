

clean: 
	$(MAKE) --directory client clean
	$(MAKE) --directory server clean

rm: 
	$(MAKE) --directory client rm
	$(MAKE) --directory server rm

fclean: 
	$(MAKE) --directory client fclean
	$(MAKE) --directory server fclean

.PHONY: fclean clean rm
