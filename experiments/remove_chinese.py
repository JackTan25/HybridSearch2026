import os
import re

def remove_chinese(text):
    """"""
    pattern = re.compile(
        r'['  # Unicode
        r'\u3000-\u303f'  # 
        r'\u4e00-\u9fff'  # 
        r'\uff00-\uffef'  # 
        r'\u3400-\u4dbf'  # A
        r']', 
        re.UNICODE
    )
    return pattern.sub('', text)

def process_directory(directory):
    allowed_ext = {'.txt', '.md', '.html', '.xml', '.log','.py'}  # 
    for root, _, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            ext = os.path.splitext(file)[1].lower()
            
            if ext not in allowed_ext:
                print(f"⚠️ : {file_path}")
                continue

            try:
                # 
                # backup_path = file_path + '.bak'
                # shutil.copy2(file_path, backup_path)

                # 
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                new_content = remove_chinese(content)
                
                # 
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(new_content)
                print(f"✅ : {file_path}")
                
            except UnicodeDecodeError:
                print(f"❌ : {file_path} ()")
            except Exception as e:
                print(f"❌ : {file_path} ({str(e)})")

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        print(": python remove_chinese.py <>")
        sys.exit(1)
    
    target_dir = sys.argv[1]
    if not os.path.isdir(target_dir):
        print(": ")
        sys.exit(1)
    
    process_directory(target_dir)
