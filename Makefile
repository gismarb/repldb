# Makefile para compilar, instalar e gerenciar o projeto repldb

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Diretórios internos do projeto
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Nome do executável final
TARGET = repldb

# Diretórios de instalação (ajustados para ambiente Firebird)
INSTALL_BIN = /opt/firebird/bin
INSTALL_DB = /opt/firebird/data

# Fontes e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# ========
# Targets
# ========

# Compilação padrão
all: $(TARGET)

# Geração do executável a partir dos objetos
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$@ $^

# Geração de cada .o a partir de cada .cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Instala o binário e o banco (caso o banco exista)
install:
	@echo "Instalando $(TARGET) em $(INSTALL_BIN)..."
	install -Dm755 $(BIN_DIR)/$(TARGET) $(INSTALL_BIN)/$(TARGET)
	@echo "Verificando diretório $(INSTALL_DB)..."
	@mkdir -p $(INSTALL_DB)
	@if [ -f repldb.fdb ]; then \
		echo "Copiando banco repldb.fdb para $(INSTALL_DB)..."; \
		install -Dm644 repldb.fdb $(INSTALL_DB)/repldb.fdb; \
		echo "Ajustando permissões do banco para firebird:firebird..."; \
		chown firebird:firebird $(INSTALL_DB)/repldb.fdb; \
	else \
		echo "Aviso: banco repldb.fdb não encontrado para copiar."; \
	fi

# Remove o binário e banco instalados
uninstall:
	@echo "Removendo $(INSTALL_BIN)/$(TARGET)..."
	rm -f $(INSTALL_BIN)/$(TARGET)
	@echo "Removendo $(INSTALL_DB)/repldb.fdb..."
	rm -f $(INSTALL_DB)/repldb.fdb

# Limpa arquivos gerados
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Limpa e recompila
rebuild: clean all

